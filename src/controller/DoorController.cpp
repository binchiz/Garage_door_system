//
// Created by zbinc on 23/02/2025.
//

#include "DoorController.h"
#include "types.h"

#include <iostream>

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
        .currentPosition = 0,
    };
    calibMargin = 300;
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

int DoorController_t::getPosition() const {
    return status.currentPosition;
}

void DoorController_t::setMoving(const bool isMoving) {
    status.moving = isMoving;
}


void DoorController_t::setCalib(const calibState data) {
    status.calibState = data;
}

void DoorController_t::setPosition(const int data) {
    status.currentPosition = data;
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
    while (!lowerLimit.isPressed()) {
        motor.moveDown();
        if (checkIfStuck()) {
            std::cout << "Calibration failed: Door stuck while moving down" << std::endl;
            return;
        }
    }
    std::cout<<"finished moving down, now start counting steps" << std::endl;
    int steps = 0;
    while (!upperLimit.isPressed()) {
        motor.moveUp();
        steps++;
        if (checkIfStuck()) {
            std::cout << "Calibration failed: Door stuck while moving up" << std::endl;
            return;
        }
    }
    for (int i = 0; i < calibMargin/2; i++) {
        motor.moveDown();
    }
    status.totalSteps = steps - calibMargin;
    status.currentPosition = 0; // Current position is calibMargin steps down from top
    status.calibState = CALIBRATED;
    status.errorState = NORMAL;
    status.doorState = GarageDoor::OPENED;
    status.moving = false;

    std::cout << "Calibration complete. Total steps: " << status.totalSteps << std::endl;
    std::cout << "Current position: " << status.currentPosition << std::endl;
}

void DoorController_t::open() {
    leds.movingLed();
    status.moving = true;
    status.doorState = GarageDoor::OPENING;
    moveStartTime = time_us_32() / 1000;
    while (status.currentPosition > 0) {
        status.currentPosition--;
        if (checkIfStuck()) return;
        motor.moveUp();
    }
    status.doorState = GarageDoor::OPENED;
    status.moving = false;

    // if (status.currentPosition > 0) {
    //     status.moving = true;
    //     status.doorState = GarageDoor::OPENING;
    //     motor.moveUp();
    //     status.currentPosition--;
    // }
    // else {
    //     status.doorState = GarageDoor::OPENED;
    //     status.moving = false;
    // }
}

void DoorController_t::close() {
    leds.movingLed();
    status.moving = true;
    status.doorState = GarageDoor::CLOSING;
    moveStartTime = time_us_32() / 1000;
    while (status.currentPosition < status.totalSteps) {
        status.currentPosition++;
        if (checkIfStuck()) return;
        motor.moveDown();
    }
    status.doorState = GarageDoor::CLOSED;
    status.moving = false;
    // if (status.currentPosition < status.totalSteps) {
    //     status.moving = true;
    //     status.doorState = GarageDoor::CLOSING;
    //     motor.moveDown();
    //     status.currentPosition++;
    // }
    // else {
    //     status.doorState = GarageDoor::CLOSED;
    //     status.moving = false;
    // }
}

void DoorController_t::stop() const {
    motor.stop();
}

void DoorController_t::controlLed() {
    if (!isCalibrated()) {
        leds.notCalibratedLed();
    } else {
        leds.idleLed();
    }
}







