#ifndef LED_H
#define LED_H

#include <pico/stdlib.h>
#include "types.h"

/*
 * - Not calibrated: LED1 blinking fast
 * - Opening/Closing: LED1 constant on
 * - Not moving (idle): LED1 slowly blinking
 * - Calibrating: 3 LEDs on
 */
class LED_t {
public:

    LED_t(uint8_t led1Pin, uint8_t led2Pin, uint8_t led3Pin);
    ~LED_t() = default;

    bool notCalibratedLed();

    void movingLed();

    bool idleLed();

    bool calibrationLed();

private:
    uint8_t led1Pin;
    uint8_t led2Pin;
    uint8_t led3Pin;
    
    absolute_time_t lastToggleTime;
    bool led1State;

    static constexpr uint32_t FAST_BLINK_MS = 100;  // Fast blinking for not calibrated
    static constexpr uint32_t SLOW_BLINK_MS = 1000; // Slow blinking for idle state

    void setLEDs(bool led1, bool led2, bool led3);

    bool toggleLED1(uint32_t rate_ms);
};

#endif // LED_H