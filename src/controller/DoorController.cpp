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
    StepperMotor_t &motor) : upperLimit(upperLimit), lowerLimit(lowerLimit), encoder(encoder), motor(motor) {
    status = {
        .doorState = GarageDoor::IDLE,
        .errorState = NORMAL,
        .calibState = UNCALIBRATED,
        .moving =false,
        .totalSteps = 0,
        .currentPosition = 0,
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

bool DoorController_t::checkIfStuck() {

    if (!encoder.hasMovedSinceLastCheck()) {
        status.moving = false;
        status.errorState = STUCK;
        status.calibState = UNCALIBRATED;
        motor.stop();
        return true;
    }
    return false;
}

void DoorController_t::calibrate() {

    while (!lowerLimit.isPressed()) {
        motor.moveDown();
    }
    int steps = 0;
    while (!upperLimit.isPressed()) {
        motor.moveUp();
        steps++;
    }
    status.totalSteps = steps;
    status.calibState = CALIBRATED;
    status.doorState = GarageDoor::OPENED;
    std::cout << status.totalSteps << std::endl;
}

void DoorController_t::open() {
    if (status.currentPosition > 0) {
        status.moving = true;
        motor.moveUp();
        status.currentPosition--;
    }
    else {
        status.doorState = GarageDoor::OPENED;
        status.moving = false;
    }
}

void DoorController_t::close() {
    if (status.currentPosition < status.totalSteps) {
        status.moving = true;
        motor.moveDown();
        status.currentPosition++;
    }
    else {
        status.doorState = GarageDoor::CLOSED;
        status.moving = false;
    }
}






