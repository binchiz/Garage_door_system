/*handles mqtt stuff
 *1. handles remote command
 *2. report status
 */
#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H
//#include <mosquitto.h>
#include "../types.h"
#include "../controller/DoorController.h"
#include "../GarageDoorSytem.h"

class GarageDoorSystem;

class MQTTHandler_t {
    public:
        explicit MQTTHandler_t(
        //...
        );
        ~MQTTHandler_t();
        void connectMQTT();
        //void handleMessage(const mosquitto_message* message);
        void publishStatus(const std::string& status);
        command createCommandFromInput();
        /*generating the correct command according to controller status
         * if opening -> close
         * if closing -> open
         * if stuck -> stop
         * ...
         */
        void update(); // call system.addCommand()
    private:
        struct mosquitto& mosq;
        bool connected{false};
        static constexpr char COMMAND_TOPIC[] = "garage/door/command";
        static constexpr char STATUS_TOPIC[] = "garage/door/status";
        static constexpr char ERROR_TOPIC[] = "garage/door/error";
        DoorController_t& controller;
        GarageDoorSystem& system;

};

#endif //MQTTHANDLER_H
