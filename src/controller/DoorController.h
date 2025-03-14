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
#include "hardware/Led.h"
#include "types.h"

class ButtonHandler_t;
class MQTTHandler_t;

class DoorController_t {
public:
    DoorController_t(
        LimitSwitch_t& upperLimit,
        LimitSwitch_t& lowerLimit,
        RotaryEncoder_t& encoder,
        StepperMotor_t& motor,
        LED_t& leds
    );

    void setButtonHandler(ButtonHandler_t* handler);
    void setMQTTHandler(MQTTHandler_t* handler);

    [[nodiscard]] GarageDoor::doorState getDoorStatus() const; // returns a structure that contains the status. see types.h
    [[nodiscard]] calibState_t isCalibrated() const;
    [[nodiscard]] bool isMoving() const;
    [[nodiscard]] int getTotalSteps() const;
    [[nodiscard]] errorState_t isStuck() const;

    void setMoving(bool isMoving);
    void setCalib(calibState data);
    void setError(errorState data);
    void setTotalSteps(int data);
    void setDoorStatus(GarageDoor::doorState data);


    void open(); // check encoder when motor running, if stuck, stop(), set errorstate and calibstate
    void close();
    void calibrate();
    void stop();
    [[nodiscard]] bool checkIfStuck(); // check if the door is actually moving, aka if the encoder is moving

    void controlLed() const;

private:
    LimitSwitch_t& upperLimit;
    LimitSwitch_t& lowerLimit;
    RotaryEncoder_t& encoder;
    StepperMotor_t& motor;
    LED_t& leds;
    Status_t status;
    uint32_t moveStartTime;
    static constexpr uint32_t STUCK_TIMEOUT = 1000;
    static constexpr int calibMargin = 300;

    ButtonHandler_t* buttonHandler = nullptr;
    MQTTHandler_t* mqttHandler = nullptr;
};

#endif //DOORCONTROLLER_H
