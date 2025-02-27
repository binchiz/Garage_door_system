/*defines our types. add more if needed*/


#ifndef TYPES_H
#define TYPES_H
#include <cstdint>
#include <string>

typedef enum command {OPEN, CLOSE, STOP, CALIB} command;
typedef enum status {OPENING, OPENED, CLOSING, CLOSEED, CALIBRATING, IDLE, ERROR} status;


struct DoorStatus {
    status status;
    int32_t currentPosition;
    bool isCalibrated;
    std::string errorMessage;
};



#endif //TYPES_H
