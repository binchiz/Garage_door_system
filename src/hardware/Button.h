#ifndef BUTTON_H
#define BUTTON_H

#include <pico/util/queue.h>

/**
 * @brief Button class for handling button press events.
 * Set the enabled parameter to true to enable the button when creating.
 * \n isPressed() returns true if the button is just pressed.
 * \n isPressing() returns true if the button is being pressed now.
 * \n  enable() and disable() can be used to enable and disable the button.
 */
class Button_t {
public:
    explicit Button_t(uint8_t pin, bool enabled=false);
    virtual ~Button_t();
    [[nodiscard]] bool isPressed();
    [[nodiscard]] bool isPressing() const;
    void enable() const;
    void disable();

private:
    const uint8_t pin;
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
    explicit LimitSwitch_t(uint8_t pin, bool enabled=false);
};

#endif // BUTTON_H