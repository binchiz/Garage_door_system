
#ifndef BUTTON_H
#define BUTTON_H
#include <cstdint>

class Button_t{
    public:
        Button_t(uint8_t pin);
        bool isPressed() const;
    private:
        uint8_t pin;
        bool lastState;
        uint32_t lastDebounceTime;
        static constexpr uint32_t DEBOUNCE_DELAY = 50;
};

class LimitSwitch_t : public Button_t {
    public:
        explicit LimitSwitch_t(uint8_t pin);
        void init();
        [[nodiscard]] bool isHit() const;

    private:
        uint8_t pos; //upper one or lower one
};


#endif //BUTTON_H
