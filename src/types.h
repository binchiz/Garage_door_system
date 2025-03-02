/*defines our types. add more if needed*/


#ifndef TYPES_H
#define TYPES_H
#include <cstdint>
#include <string>

typedef enum command {OPEN, CLOSE, STOP, CALIB} command;

typedef enum doorState {OPENING, OPENED, CLOSING, CLOSED} doorState_t;
typedef enum errorState {NORMAL, STUCK} errorState_t;
typedef enum calibState {UNCALIBRATED, CALIBRATED} calibState_t;

struct Status_t {
    doorState_t doorState;
    errorState_t errorState;
    calibState_t calibState;
    int32_t currentPosition;
};



#endif //TYPES_H
