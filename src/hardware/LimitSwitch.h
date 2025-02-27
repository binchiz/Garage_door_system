
#ifndef LIMIT_SWITCH_H
#define LIMIT_SWITCH_H
#include <cstdint>

class LimitSwitch {
public:
    explicit LimitSwitch(uint8_t pin);
    void init();
    [[nodiscard]] bool isHit() const;

private:
    uint8_t pin;
    uint8_t pos; //upper one or lower one
};

#endif //LIMIT_SWITCH_H
