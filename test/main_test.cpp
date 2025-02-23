#include <iostream>
#include "pico/stdlib.h"

int main() {

    const uint led_pin = 22;
    uint count = 0;

    // Initialize LED pin
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    // Initialize chosen serial port
    stdio_init_all();

    std::cout << "\nBoot\n";

    // Loop forever
    while (true) {

        // Blink LED
        std::cout << "Blinking! " << ++count << "\n";
        gpio_put(led_pin, true);
        sleep_ms(1000);
        gpio_put(led_pin, false);
        sleep_ms(1000);
    }
}