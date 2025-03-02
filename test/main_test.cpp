#include <iostream>
#include "pico/stdlib.h"
#include "hardware/StepperMotor.h"

int main() {

    StepperMotor_t motor(2, 3, 6, 13);
    motor.init();
    // Initialize chosen serial port
    stdio_init_all();

    std::cout << "\nBoot\n";

    // Loop forever
    while (true) {

        motor.moveDown();

    }
}