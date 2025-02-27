
#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H
#include <array>
#include <cstdint>

class StepperMotor {
public:
    StepperMotor(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4);
    void moveUp();
    void moveDown();
    void stop();

private:
    enum class Move { UP, DOWN, STOP };
    std::array<uint8_t, 4> pins;
    std::array<uint8_t, 8> step_bits;
    uint8_t step;
    Move currentMove;
    void setMove(Move dir);
};

#endif //STEPPERMOTOR_H
