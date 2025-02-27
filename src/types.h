/*defines our types. add more if needed*/


#ifndef TYPES_H
#define TYPES_H
#include <cstdint>
#include <string>

typedef enum command {OPEN, CLOSE, STOP, CALIB} command;

typedef enum doorState {OPENING, OPENED, CLOSING, CLOSED} doorState;
typedef enum errorState {NORMAL, STUCK} errorState;
typedef enum calibState {UNCALIBRATED, CALIBRATED} calibState;

struct Status {
    doorState doorState;
    errorState errorState;
    calibState calibState;
    int32_t currentPosition;
};



#endif //TYPES_H
