//
// Created by zbinc on 26/02/2025.
//

#include "Button.h"
#include <pico/stdlib.h>
#include "handler/GPIOInterrupt.h"

Button_t::Button_t(const uint8_t pin) : pin(pin), lastDebounceTime(0), pressing(false) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
    queue_init(&button_event_queue, sizeof(bool), 10);
    GPIOInterrupt::registerCallback(pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, &Button_t::staticIrqCallback, this);

}

bool Button_t::isPressed() {
    bool pressed = false;
    queue_try_remove(&button_event_queue, &pressed);
    return pressed;
}

bool Button_t::isPressing() const {
    return pressing;
}

Button_t::~Button_t() {
    GPIOInterrupt::unregisterCallback(pin);
}


void Button_t::staticIrqCallback(void *context, const uint32_t events) {
    Button_t *self = static_cast<Button_t *>(context);
    self->irqCallback(events);
}

void Button_t::irqCallback(const uint32_t events) {
    if (events & GPIO_IRQ_EDGE_FALL) {
        if (time_us_32() - lastDebounceTime > DEBOUNCE_DELAY * 1000) {
            lastDebounceTime = time_us_32();
            pressing = true;
            queue_try_add(&button_event_queue, &pressing);
        }
    }
    if (events & GPIO_IRQ_EDGE_RISE) {
        lastDebounceTime = time_us_32();
        pressing = false;
    }
}

void Button_t::enable() const {
    GPIOInterrupt::enable(pin);
}

void Button_t::disable() const {
    GPIOInterrupt::disable(pin);
}


LimitSwitch_t::LimitSwitch_t(const uint8_t pin) : Button_t(pin) {
}