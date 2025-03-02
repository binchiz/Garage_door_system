#include "StepperMotor.h"

#include <hardware/gpio.h>
#include <pico/time.h>

StepperMotor_t::StepperMotor_t(const uint8_t pin1, const uint8_t pin2, const uint8_t pin3, const uint8_t pin4) {
    pins[0] = pin1;
    pins[1] = pin2;
    pins[2] = pin3;
    pins[3] = pin4;
    step_bits = {0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001};
    step = 0;
}

void StepperMotor_t::init() const {
    for (const uint8_t pin : pins) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);
    }
}

void StepperMotor_t::moveUp() {
    step = (step + 1 ) % 8;
    const uint next_step = step_bits[step];
    for (size_t i = 0; i < pins.size(); ++i) {
        gpio_put(pins[i], (next_step >> i) & 1);
    }
    sleep_ms(2);
}

void StepperMotor_t::moveDown() {
    step = (step + 7 ) % 8;
    const uint next_step = step_bits[step];
    for (size_t i = 0; i < pins.size(); ++i) {
        gpio_put(pins[i], (next_step >> i) & 1);
    }
    sleep_ms(2);
}

void StepperMotor_t::stop() const {
    for (const uint8_t pin : pins) {
        gpio_put(pin, false);
    }
}

