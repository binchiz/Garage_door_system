#include <pico/stdlib.h>
#include <iostream>
#include "controller/DoorController.h"
#include "hardware/Button.h"
#include "hardware/RotaryEncoder.h"
#include "hardware/StepperMotor.h"


int main() {
    LimitSwitch_t upper(16);
    LimitSwitch_t lower(17);
    RotaryEncoder_t encoder(28, 27);
    StepperMotor_t stepperMotor(2,3,6,13);
    DoorController_t controller(upper, lower, encoder, stepperMotor);

    lower.disable();
    upper.enable();
    stdio_init_all();

    controller.calibrate();
}
