#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

#include <pico/util/queue.h>

/**
 * @brief Rotary encoder class for handling encoder rotating events.
 * Set the enabled parameter to true to enable the encoder when creating.
 * \n getPosition() returns the current position. It is relative from the last reset, it may have minus position.
 * \n hasMovedSinceLastCheck() returns true if the encoder has moved since the last check.
 * \n resetPosition() resets the position to 0.
 * \n  enable() and disable() can be used to enable and disable the button.
 */
class RotaryEncoder_t {
public:
    // Set the enabled parameter to true to enable the encoder when creating.
    RotaryEncoder_t(uint8_t pinA, uint8_t pinB, bool enabled = false);
    ~RotaryEncoder_t();
    [[nodiscard]] int getPosition(); // The position is converted from uint8_t, range 0 - 255.
    bool hasMovedSinceLastCheck();
    void resetPosition();
    void enable() const;
    void disable() const;

private:
    const uint8_t pinA, pinB; //ADC1
    volatile uint8_t position; // uint8_t will loop back when out of range, no worry about overflow.
    int lastCheckedPosition;
    queue_t eventQueue;
    // According to the length of the belt, the maximum position change is about 70,
    // so the queue size of 100 is far enough, even if the caller doesn't call to update the position frequently.
    static constexpr uint EVENT_QUEUE_SIZE = 100;

    static void staticIrqCallback(void *context, uint32_t events);
    void irqCallback(uint32_t events);

};

#endif //ROTARYENCODER_H
