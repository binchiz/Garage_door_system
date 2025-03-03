/*
* responsible for controlling the hardware only. no system logic is implemented here
* takes reference/pointer? to hardware components as member
* no eeprom here since we are not handing the storage here
* keep track of door status
 */
#ifndef DOORCONTROLLER_H
#define DOORCONTROLLER_H
#include "../hardware/RotaryEncoder.h"
#include "../hardware/StepperMotor.h"
#include "../hardware/Button.h"
#include "../types.h"
#include <memory>

class DoorController_t {
public:
    DoorController_t(
        std::unique_ptr<LimitSwitch_t> upperLimit,
        std::unique_ptr<LimitSwitch_t> lowerLimit,
        std::unique_ptr<RotaryEncoder_t> encoder,
        std::unique_ptr<StepperMotor_t> motor
    );

    Status_t getDoorStatus(); // returns a structure that contains the status. see types.h
    void setDoorStatus(); // could be divided into smaller methods for doorstate, errorstate, calibstate, position

    void open(); // check encoder when motor running, if stuck, stop(), set errorstate and calibstate
    void close();
    void calibrate();
    void stop();
    [[nodiscard]] bool checkMoving() const; // check if the door is actually moving, aka if the encoder is moving

private:
    std::unique_ptr<LimitSwitch_t> upperLimit;
    std::unique_ptr<LimitSwitch_t> lowerLimit;
    std::unique_ptr<RotaryEncoder_t> encoder;
    std::unique_ptr<StepperMotor_t> motor;
    int totalStep;
    Status_t Status;

};

#endif //DOORCONTROLLER_H
