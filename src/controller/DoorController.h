/*
* responsible for controlling the hardware only. no system logic is implemented here
* takes reference/pointer? to hardware components as member
* no eeprom here since we are not handing the storage here
* keep track of door status
 */
#ifndef DOORCONTROLLER_H
#define DOORCONTROLLER_H
#include "hardware/RotaryEncoder.h"
#include "hardware/StepperMotor.h"
#include "hardware/Button.h"
#include "types.h"

class DoorController_t {
public:
    DoorController_t(
        LimitSwitch_t& upperLimit,
        LimitSwitch_t& lowerLimit,
        RotaryEncoder_t& encoder,
        StepperMotor_t& motor
    );

    [[nodiscard]] GarageDoor::doorState getDoorStatus() const; // returns a structure that contains the status. see types.h
    [[nodiscard]] bool isCalibrated() const;
    bool isMoving() const;
    void open(); // check encoder when motor running, if stuck, stop(), set errorstate and calibstate
    void close();
    void calibrate();
    void stop();
    [[nodiscard]] bool checkIfStuck(); // check if the door is actually moving, aka if the encoder is moving

private:
    LimitSwitch_t& upperLimit;
    LimitSwitch_t& lowerLimit;
    RotaryEncoder_t& encoder;
    StepperMotor_t& motor;
    Status_t status;

};

#endif //DOORCONTROLLER_H
