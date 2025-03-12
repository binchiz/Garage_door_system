/*defines our types. add more if needed*/


#ifndef TYPES_H
#define TYPES_H
#include <cstdint>
#include <string>

typedef enum command {OPEN, CLOSE, STOP, CALIB, NONE} command;

namespace GarageDoor {
    typedef enum doorState {OPENING, OPENED, CLOSING, CLOSED, IDLE} doorState_t;
    inline const char* getDoorStateString(doorState_t state) {
        switch (state) {
            case OPENING:  return "OPENING";
            case OPENED:   return "OPENED";
            case CLOSING:  return "CLOSING";
            case CLOSED:   return "CLOSED";
            case IDLE:     return "IDLE";
            default:       return "UNKNOWN";
        }
    }
}
typedef enum errorState {NORMAL, STUCK} errorState_t;
typedef enum calibState {UNCALIBRATED, CALIBRATED} calibState_t;

struct Status_t {
    GarageDoor::doorState_t doorState;
    errorState_t errorState; //卡每卡住
    calibState_t calibState;
    bool moving;
    int totalSteps;
};

inline const char* getErrorStateString(errorState_t state) {
    switch (state) {
        case NORMAL:  return "NORMAL";
        case STUCK:   return "STUCK";
        default:      return "UNKNOWN";
    }
}

inline const char* getCalibStateString(calibState_t state) {
    switch (state) {
        case UNCALIBRATED:  return "UNCALIBRATED";
        case CALIBRATED:    return "CALIBRATED";
        default:            return "UNKNOWN";
    }
}

namespace MQTTTMessage {
    constexpr const char* STATUS_TOPIC = "garage/door/status";
    constexpr const char* COMMAND_TOPIC = "garage/door/command";
    constexpr const char* RESPONSE_TOPIC = "garage/door/response";
    constexpr const char* REBOOT_MSG = "SYSTEM REBOOT";
    constexpr const char* SUCCESS_MSG = "SUCCESS";
    constexpr const char* STUCK_MSG = "DOOR STUCK";
    constexpr const char* NO_CALIB_MSG = "NOT CALIBRATED";
}





#endif //TYPES_H
