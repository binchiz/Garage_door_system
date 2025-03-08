//
// Created by Sheng Tai on 07/03/2025.
//

#ifndef GARAGE_DOOR_SYSTEM_GPIOINTERRUPT_H
#define GARAGE_DOOR_SYSTEM_GPIOINTERRUPT_H

#include <pico/stdlib.h>

class GPIOInterrupt {
public:
    using IrqCallback = void (*)(void *context, uint32_t events);
    static void init();
    static void registerCallback(uint pin, uint32_t events, IrqCallback callback, void *context);
    static void unregisterCallback(uint pin);
    static void enable(uint pin);
    static void disable(uint pin);

private:
    static constexpr int MAX_GPIO_PINS = 30;
    static IrqCallback callbacks[MAX_GPIO_PINS];
    static uint32_t callbacksEvents[MAX_GPIO_PINS];
    static void *contexts[MAX_GPIO_PINS];
    static void globalGPIOCallback(uint pin, uint32_t events);

};

#endif // GARAGE_DOOR_SYSTEM_GPIOINTERRUPT_H