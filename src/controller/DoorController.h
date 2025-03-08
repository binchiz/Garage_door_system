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

    Status_t getDoorStatus(); // returns a structure that contains the status. see types.h
    // void setDoorState(GarageDoor::doorState_t state);
    // void setCalibState(calibState_t state);
    // void setErrorState(errorState_t state);
    // void setTotalSteps(int steps);
    // void setPosition(int position);// could be divided into smaller methods for doorstate, errorstate, calibstate, position

    void open(); // check encoder when motor running, if stuck, stop(), set errorstate and calibstate
    void close();
    void calibrate();
    void stop();
    [[nodiscard]] bool checkMoving() const; // check if the door is actually moving, aka if the encoder is moving

private:
    LimitSwitch_t& upperLimit;
    LimitSwitch_t& lowerLimit;
    RotaryEncoder_t& encoder;
    StepperMotor_t& motor;
    Status_t status;

};

#endif //DOORCONTROLLER_H
