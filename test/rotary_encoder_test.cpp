//
// Created by Sheng Tai on 08/03/2025.
//

#include <pico/stdlib.h>
#include <iostream>
#include "hardware/Button.h"
#include "hardware/RotaryEncoder.h"
#include "handler/GPIOInterrupt.h"
#include "hardware/StepperMotor.h"

int main() {
    stdio_init_all();
    GPIOInterrupt::init();

    std::cout << "Rotary Encoder Test" << std::endl;

    Button_t button0(9, true);
    Button_t button1(8, true);
    Button_t button2(7, true);
    RotaryEncoder_t rotaryEncoder(27, 28, true);
    StepperMotor_t motor(2, 3, 6, 13);

    std::cout << "Press sw_0/sw_2 to move the motor up/down\n";
    std::cout << "Press sw_1 to check if the rotary encoder moved or not\n";

    while (true) {
        if (button0.isPressing()) {
            motor.moveUp();
        }
        else{
            motor.stop();
        }

        if (button2.isPressing()) {
            motor.moveDown();
        }
        else{
            motor.stop();
        }

        if (button1.isPressed()) {
            std::cout << "Rotary encoder position: " << static_cast<int>(rotaryEncoder.getPosition()) << std::endl;
            std::cout << "Rotary encoder has moved: " << rotaryEncoder.hasMovedSinceLastCheck() << std::endl;
        }
    }
    return 0;
}