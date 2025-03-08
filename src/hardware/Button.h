#ifndef BUTTON_H
#define BUTTON_H
#include <cstdint>
#include <pico/util/queue.h>


/**
 * @brief Button class for handling button press events.
 * The button is not enabled by default. After creating the button instance, call enable() to enable the button.
 * isPressed() returns true if the button is just pressed.
 * isPressing() returns true if the button is being pressed now.
 */

class Button_t {
public:
    explicit Button_t(uint8_t pin);
    ~Button_t();
    [[nodiscard]] bool isPressed();
    [[nodiscard]] bool isPressing() const;
    void enable() const;
    void disable();

private:
    uint8_t pin;
    volatile uint32_t lastDebounceTime;
    volatile bool pressing;
    queue_t eventQueue;
    static constexpr uint32_t DEBOUNCE_DELAY = 100; // ms
    static constexpr uint EVENT_QUEUE_SIZE = 30;

    static void staticIrqCallback(void *context, uint32_t events);
    void irqCallback(uint32_t events);
};

class LimitSwitch_t : public Button_t {
public:
    explicit LimitSwitch_t(uint8_t pin);
};

#endif // BUTTON_H