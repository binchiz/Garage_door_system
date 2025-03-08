#ifndef BUTTON_H
#define BUTTON_H
#include <cstdint>
#include <pico/util/queue.h>

class Button_t {
public:
    explicit Button_t(uint8_t pin);
    ~Button_t();
    [[nodiscard]] bool isPressed();
    [[nodiscard]] bool isPressing() const;
    void enable() const;
    void disable() const;

private:
    uint8_t pin;
    volatile uint32_t lastDebounceTime;
    volatile bool pressing;
    queue_t button_event_queue;
    static constexpr uint32_t DEBOUNCE_DELAY = 100; // ms
    static void staticIrqCallback(void *context, uint32_t events);
    void irqCallback(uint32_t events);
};

class LimitSwitch_t : public Button_t {
public:
    explicit LimitSwitch_t(uint8_t pin);
};

#endif // BUTTON_H