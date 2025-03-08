
#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H
#include <array>
#include <cstdint>

class StepperMotor_t {
public:
    explicit StepperMotor_t(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
    void moveUp();
    void moveDown();
    void stop() const;

private:
    std::array<uint8_t, 4> pins{};
    std::array<uint8_t, 8> step_bits{};
    uint8_t step;
};

#endif //STEPPERMOTOR_H
