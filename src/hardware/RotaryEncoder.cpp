//
// Created by zbinc on 23/02/2025.
//

#include "RotaryEncoder.h"
#include <pico/stdlib.h>
#include "handler/GPIOInterrupt.h"

RotaryEncoder_t::RotaryEncoder_t(uint8_t pinA, uint8_t pinB, bool enabled)
        : pinA(pinA), pinB(pinB), position(0), lastCheckedPosition(0), eventQueue{} {
    gpio_init(pinA);
    gpio_set_dir(pinA, GPIO_IN);

    gpio_init(pinB);
    gpio_set_dir(pinB, GPIO_IN);

    queue_init(&eventQueue, sizeof(int), EVENT_QUEUE_SIZE);
    GPIOInterrupt::registerCallback(pinA, GPIO_IRQ_EDGE_RISE, &RotaryEncoder_t::staticIrqCallback, this);
    if (enabled) {
        enable();
    }
}

RotaryEncoder_t::~RotaryEncoder_t() {
    GPIOInterrupt::unregisterCallback(pinA);
}

int RotaryEncoder_t::getPosition() {
    int posChange;
    while (queue_try_remove(&eventQueue, &posChange)) {
        position += posChange;
    }
    return position;
}

bool RotaryEncoder_t::hasMovedSinceLastCheck() {
    int latestPosition = getPosition();
    if (lastCheckedPosition != latestPosition) {
        lastCheckedPosition = latestPosition;
        return true;
    } else {
        return false;
    }
}

void RotaryEncoder_t::resetPosition() {
    position = 0;
}

void RotaryEncoder_t::enable() const {
    GPIOInterrupt::enableCallback(pinA);
}

void RotaryEncoder_t::disable() const {
    GPIOInterrupt::disableCallback(pinA);
}

void RotaryEncoder_t::staticIrqCallback(void *context, uint32_t events) {
    RotaryEncoder_t *self = static_cast<RotaryEncoder_t *>(context);
    self->irqCallback(events);
}

void RotaryEncoder_t::irqCallback(uint32_t events) {
    if (gpio_get(pinB)) {
        int direction = 1;
        queue_try_add(&eventQueue, &direction);
    } else {
        int direction = -1;
        queue_try_add(&eventQueue, &direction);
    }
}