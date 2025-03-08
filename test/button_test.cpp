//
// Created by Sheng Tai on 08/03/2025.
//

#include <pico/stdlib.h>
#include <iostream>
#include "hardware/Button.h"
#include "handler/GPIOInterrupt.h"

int main() {
    stdio_init_all();
    GPIOInterrupt::init(); // init GPIO interrupt, must be called before using GPIOInterrupt

    std::cout << "Boot\n";

    Button_t button0(9);
    Button_t button1(8);
    Button_t button2(7);

    LimitSwitch_t limitSwitchLower(16);
    LimitSwitch_t limitSwitchUpper(17);

    button0.enable();
    button1.enable();
    button2.enable();
    limitSwitchLower.enable();
    limitSwitchUpper.enable();

    std::cout << "Press sw_1 to test button interrupt\n";
    std::cout << "Press sw_0 and sw_2 together to calib (wait 2s)\n";
    int count = 0;

    uint32_t calibStartTime = 0;
    bool calibrating = false;

    while (true) {
        if (button1.isPressed()) {
            std::cout << "Button 1 pressed: " << count++ << std::endl;
        }

        if (limitSwitchLower.isPressed()) {
            std::cout << "Lower limit switch pressed\n";
        }
        if (limitSwitchUpper.isPressed()) {
            std::cout << "Upper limit switch pressed\n";
        }

        uint32_t currentTime = time_us_32();
        if ((button0.isPressed() && button2.isPressing()) || (button2.isPressed() && button0.isPressing())) {

            // calib
            std::cout << "Calib\n";
            button0.disable();
            button2.disable();
            calibStartTime = currentTime;
            calibrating = true;
        }

        if (calibrating) {
            if (currentTime - calibStartTime > 2000000) {
                button0.enable();
                button2.enable();
                calibrating = false;
                std::cout << "Calib finished\n";
            }
        }
    }
    return 0;
}