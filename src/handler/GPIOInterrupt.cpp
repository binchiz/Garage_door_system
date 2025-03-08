//
// Created by Sheng Tai on 07/03/2025.
//

#include "GPIOInterrupt.h"
#include <iostream>

GPIOInterrupt::IrqCallback GPIOInterrupt::callbacks[MAX_GPIO_PINS] = {nullptr};
uint32_t GPIOInterrupt::callbacksEvents[MAX_GPIO_PINS] = {0};
void *GPIOInterrupt::contexts[MAX_GPIO_PINS] = {nullptr};

void GPIOInterrupt::init() {
    gpio_set_irq_callback(globalGPIOCallback);
    irq_set_enabled(IO_IRQ_BANK0, true);
}

void GPIOInterrupt::registerCallback(const uint pin, const uint32_t events, const IrqCallback callback,
                                     void *context) {
    assert(pin < MAX_GPIO_PINS); // GPIO number error
    assert(callbacks[pin] == nullptr); // callback already registered
    callbacks[pin] = callback;
    contexts[pin] = context;
    callbacksEvents[pin] = events;
}

void GPIOInterrupt::unregisterCallback(const uint pin) {
    assert(pin < MAX_GPIO_PINS); // GPIO number error
    assert(callbacks[pin] != nullptr); // callback not registered
    gpio_set_irq_enabled(pin, callbacksEvents[pin], false);
    callbacks[pin] = nullptr;
    contexts[pin] = nullptr;
    callbacksEvents[pin] = 0;
}

void GPIOInterrupt::enableCallback(uint pin) {
    assert(pin < MAX_GPIO_PINS); // GPIO number error
    assert(callbacks[pin] != nullptr); // callback not registered
    gpio_set_irq_enabled(pin, callbacksEvents[pin], true);
}

void GPIOInterrupt::disableCallback(uint pin) {
    assert(pin < MAX_GPIO_PINS); // GPIO number error
    assert(callbacks[pin] != nullptr); // callback not registered
    gpio_set_irq_enabled(pin, callbacksEvents[pin], false);
}

void GPIOInterrupt::globalGPIOCallback(const uint pin, const uint32_t events) {
    assert(pin < MAX_GPIO_PINS); // GPIO number error
    assert(callbacks[pin] != nullptr); // callback not registered
    callbacks[pin](contexts[pin], events);
}