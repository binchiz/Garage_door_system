#include "LED.h"

LED_t::LED_t(const uint8_t led1Pin, const uint8_t led2Pin, const uint8_t led3Pin)
    : led1Pin(led1Pin), led2Pin(led2Pin), led3Pin(led3Pin), led1State(false) {

    gpio_init(led1Pin);
    gpio_set_dir(led1Pin, GPIO_OUT);

    gpio_init(led2Pin);
    gpio_set_dir(led2Pin, GPIO_OUT);

    gpio_init(led3Pin);
    gpio_set_dir(led3Pin, GPIO_OUT);

    setLEDs(false, false, false);
    lastToggleTime = get_absolute_time();
}


bool LED_t::notCalibratedLed() {
    gpio_put(led2Pin, false);
    gpio_put(led3Pin, false);

    return toggleLED1(FAST_BLINK_MS);
}

void LED_t::movingLed() {
    gpio_put(led1Pin, true);
    led1State = true;

    gpio_put(led2Pin, false);
    gpio_put(led3Pin, false);
}

bool LED_t::idleLed() {
    gpio_put(led2Pin, false);
    gpio_put(led3Pin, false);

    return toggleLED1(SLOW_BLINK_MS);
}

bool LED_t::calibrationLed() {
    gpio_put(led1Pin, true);
    gpio_put(led2Pin, true);
    gpio_put(led3Pin, true);
    led1State = true;

    return false;
}

void LED_t::setLEDs(bool led1, bool led2, bool led3) {
    gpio_put(led1Pin, led1);
    gpio_put(led2Pin, led2);
    gpio_put(led3Pin, led3);
    led1State = led1;
}

bool LED_t::toggleLED1(uint32_t rate_ms) {
    absolute_time_t currentTime = get_absolute_time();

    if (absolute_time_diff_us(lastToggleTime, currentTime) >= rate_ms * 1000) {
        led1State = !led1State;
        gpio_put(led1Pin, led1State);
        lastToggleTime = currentTime;
        return true;
    }

    return false;
}