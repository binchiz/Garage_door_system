#include <iostream>
#include "pico/stdlib.h"
#include "hardware/StepperMotor.h"


bool button_pressed(uint btn);
void calib(int& steps, StepperMotor_t& motor);
int main() {

    StepperMotor_t motor(2, 3, 6, 13);

    // Initialize chosen serial port
    stdio_init_all();

    std::cout << "\nBoot\n";

    int steps = 0;

    // Loop forever
    while (true) {
        motor.moveDown();
    }
}

