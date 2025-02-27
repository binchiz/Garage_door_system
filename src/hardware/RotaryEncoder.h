
#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

#include <cstdint>

class RotaryEncoder {
public:
    RotaryEncoder(uint8_t pinA, uint8_t pinB);
    uint32_t getPosition() const;
    bool hasMovedSinceLastCheck();
    void reset();

private:
    uint8_t pinA, pinB;
    volatile int32_t position;
    int32_t lastCheckedPosition;

    static void handleInterrupt(uint8_t gpio, uint32_t events, void* ctx);
};

#endif //ROTARYENCODER_H
