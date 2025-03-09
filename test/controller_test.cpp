#include <pico/stdlib.h>
#include <iostream>
#include "controller/DoorController.h"
#include "hardware/Button.h"
#include "hardware/RotaryEncoder.h"
#include "hardware/StepperMotor.h"
#include "handler/GPIOInterrupt.h"


int main() {
    LimitSwitch_t upper(16, true);
    LimitSwitch_t lower(17, true);
    Button_t button0(9, true);
    Button_t button1(8, true);
    Button_t button2(7, true);
    RotaryEncoder_t encoder(28, 27, true);
    StepperMotor_t stepperMotor(2,3,6,13);
    DoorController_t controller(upper, lower, encoder, stepperMotor);
    GPIOInterrupt::init();

    stdio_init_all();

    controller.calibrate();

    while (true) {
        // bool calibrated = false;
        // if ((button0.isPressed() && button2.isPressing()) || (button2.isPressed() && button0.isPressing())) {
        //     std::cout << "button pressed" << std::endl;
        //     controller.calibrate();
        //     calibrated = true;
        // }
        // if (calibrated) {
            bool close = false;
            if (button1.isPressed()) {
                close = true;
                while (close) {
                    controller.close();
                    if (button1.isPressed()) close = false;
                    if (!controller.isMoving()) close = false;
                }
            }
        }

}
