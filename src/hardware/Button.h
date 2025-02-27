
#ifndef BUTTON_H
#define BUTTON_H
#include <cstdint>

class Button {
    public:
        Button(uint8_t pin);
        bool isPressed() const;
    private:
        uint8_t pin;
        bool lastState;
        uint32_t lastDebounceTime;
        static constexpr uint32_t DEBOUNCE_DELAY = 50;
};


#endif //BUTTON_H
