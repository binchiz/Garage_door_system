//
// Created by zbinc on 23/02/2025.
//

#include "DoorController.h"
#include "handler/ButtonHandler.h"
#include "handler/MQTTHandler.h"
#include "types.h"


DoorController_t::DoorController_t(
    LimitSwitch_t &upperLimit,
    LimitSwitch_t &lowerLimit,
    RotaryEncoder_t &encoder,
    StepperMotor_t &motor,
    LED_t& leds) : upperLimit(upperLimit), lowerLimit(lowerLimit), encoder(encoder), motor(motor), leds(leds) {
    status = {
        .doorState = GarageDoor::IDLE,
        .errorState = NORMAL,
        .calibState = UNCALIBRATED,
        .moving =false,
        .totalSteps = 0,
    };
}

void DoorController_t::setButtonHandler(ButtonHandler_t* handler) {
    buttonHandler = handler;
}

void DoorController_t::setMQTTHandler(MQTTHandler_t* handler) {
    mqttHandler = handler;
}

GarageDoor::doorState DoorController_t::getDoorStatus() const {
    return status.doorState;
}

calibState_t DoorController_t::isCalibrated() const {
    return status.calibState;
}

bool DoorController_t::isMoving() const {
    return status.moving;
}

int DoorController_t::getTotalSteps() const {
    return status.totalSteps;
}


void DoorController_t::setMoving(const bool isMoving) {
    status.moving = isMoving;
}


void DoorController_t::setCalib(const calibState data) {
    status.calibState = data;
}


void DoorController_t::setError(const errorState data) {
    status.errorState = data;
}

void DoorController_t::setTotalSteps(const int data) {
    status.totalSteps = data;
}

void DoorController_t::setDoorStatus(const GarageDoor::doorState data) {
    status.doorState = data;
}


bool DoorController_t::checkIfStuck() {
    uint32_t currentTime = time_us_32()/1000;

    // If this is the first check after starting to move, initialize lastMovementTime
    if (status.moving && moveStartTime == 0) {
        moveStartTime = currentTime;
        return false;
    }

    // If the encoder has moved, update the last movement time
    if (encoder.hasMovedSinceLastCheck()) {
        moveStartTime = currentTime;
        return false;
    }

    // If we're not moving, reset lastMovementTime and return
    if (!status.moving) {
        moveStartTime = 0;
        return false;
    }

    // Calculate time elapsed since last movement (in milliseconds)
    uint32_t timeElapsed = (currentTime - moveStartTime);

    // If we're supposed to be moving but haven't moved for a while, declare stuck
    if (timeElapsed >= STUCK_TIMEOUT) {
        status.moving = false;
        status.errorState = STUCK;
        status.calibState = UNCALIBRATED;
        motor.stop();
        moveStartTime = 0; // Reset for next movement
        return true;
    }
    return false;

}

errorState_t DoorController_t::isStuck() const {
    return status.errorState;
}

void DoorController_t::calibrate() {
    leds.calibrationLed();
    status.moving = true;
    moveStartTime = time_us_32() / 1000;
    if (!gpio_get(17)) {
        for (int i = 0; i < calibMargin; i++) motor.moveUp();
    }
    while (!lowerLimit.isPressed()) {
        motor.moveDown();
        if (checkIfStuck()) {
            return;
        }
    }
    int steps = 0;
    while (!upperLimit.isPressed()) {
        motor.moveUp();
        steps++;
        if (checkIfStuck()) {
            return;
        }
    }
    status.totalSteps = steps;
    status.calibState = CALIBRATED;
    status.errorState = NORMAL;
    status.doorState = GarageDoor::OPENED;
    status.moving = false;
}

void DoorController_t::open() {
    leds.movingLed();
    status.moving = true;
    status.doorState = GarageDoor::OPENING;
    moveStartTime = time_us_32() / 1000;
    while (!upperLimit.isPressed()) {
        if (checkIfStuck()) return;
        motor.moveUp();
        buttonHandler->update();
        mqttHandler->yield_MQTT(1);
        if (!isMoving()) return;
    }
    status.doorState = GarageDoor::OPENED;
    status.moving = false;
}

void DoorController_t::close() {
    leds.movingLed();
    status.moving = true;
    status.doorState = GarageDoor::CLOSING;
    moveStartTime = time_us_32() / 1000;
    while (!lowerLimit.isPressed()) {
        if (checkIfStuck()) return;
        motor.moveDown();
        buttonHandler->update();
        mqttHandler->yield_MQTT(1);
        if (!isMoving()) return;
    }
    status.doorState = GarageDoor::CLOSED;
    status.moving = false;
}

void DoorController_t::stop() {
    motor.stop();
    status.moving = false;
}

void DoorController_t::controlLed() {
    if (!isCalibrated()) {
        leds.notCalibratedLed();
    } else {
        leds.idleLed();
    }
}







