
#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

#include <pico/util/queue.h>

class RotaryEncoder_t {
public:
    RotaryEncoder_t(uint8_t pinA, uint8_t pinB);
    [[nodiscard]] int getPosition();
    bool hasMovedSinceLastCheck();
    void resetPosition();
    void enable() const;
    void disable() const;

private:
    uint8_t pinA, pinB; //ADC1
    volatile int position; // using int, it may have minus position (relative position from the restart)
    int lastCheckedPosition;
    queue_t eventQueue;
    // According to the length of the belt, the maximum position change is about 70,
    // so the queue size of 100 is far enough, even if the caller doesn't call to update the position frequently.
    static constexpr uint EVENT_QUEUE_SIZE = 100;

    static void staticIrqCallback(void *context, uint32_t events);
    void irqCallback(uint32_t events);

};

#endif //ROTARYENCODER_H
