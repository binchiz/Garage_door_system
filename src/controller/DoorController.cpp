//
// Created by zbinc on 23/02/2025.
//

#include "DoorController.h"
#include "types.h"

DoorController_t::DoorController_t(
    LimitSwitch_t &upperLimit,
    LimitSwitch_t &lowerLimit,
    RotaryEncoder_t &encoder,
    StepperMotor_t &motor) : upperLimit(upperLimit), lowerLimit(lowerLimit), encoder(encoder), motor(motor) {
    status = {
        .doorState = GarageDoor::IDLE,
        .errorState = NORMAL,
        .calibState = UNCALIBRATED,
        .totalSteps = 0,
        .currentPosition = 0,
        .moving =false,
    };
}

GarageDoor::doorState DoorController_t::getDoorStatus() const {
    return status.doorState;
}

bool DoorController_t::isCalibrated() const {
    return status.calibState;
}

bool DoorController_t::isMoving() const {
    return status.moving;
}


void DoorController_t::calibrate() {
    motor.moveDown();
    while (!lowerLimit.isPressed()) {
        sleep_ms(10);
    }
    motor.stop();
    sleep_ms(200);
    int steps = 0;
    motor.moveUp();
    while (!upperLimit.isPressed()) {
        steps++;
    }
    motor.stop();
    status.totalSteps = steps;
    status.calibState = CALIBRATED;
    status.doorState = GarageDoor::OPENED;
    status.errorState = NORMAL;
    status.currentPosition = 0;
}



