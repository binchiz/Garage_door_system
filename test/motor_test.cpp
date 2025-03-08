#include <iostream>
#include "pico/stdlib.h"
#include "hardware/StepperMotor.h"


bool button_pressed(uint btn);
void calib(int& steps, StepperMotor_t& motor);
int main() {

    StepperMotor_t motor(2, 3, 6, 13);
    gpio_init(16);
    gpio_set_dir(16, GPIO_IN);
    gpio_pull_up(16);
    gpio_init(17);
    gpio_set_dir(17, GPIO_IN);
    gpio_pull_up(17);
    motor.init();
    // Initialize chosen serial port
    stdio_init_all();

    std::cout << "\nBoot\n";

    int steps = 0;

    // Loop forever
    while (true) {
        calib(steps, motor);
        std::cout << steps << "\n";
    }
}

void calib(int& steps, StepperMotor_t& motor) {
    motor.moveDown();
    while (!button_pressed(17)) {};
    while (!button_pressed(16)) {
        motor.moveUp();
        steps++;
    }
    motor.stop();
}

bool button_pressed(uint btn) {
    int press = 0;
    int release = 0;
    while (press < 3 && release < 3) {
        if (!gpio_get(btn)) {
            press++;
            release = 0;
        } else {
            release++;
            press = 0;
        }
        sleep_ms(10);
    }
    return (press > release);
}