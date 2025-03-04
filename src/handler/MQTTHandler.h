/*handles mqtt stuff
 *1. handles remote command
 *2. report status
 */
#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H
//#include <mosquitto.h>
#include <MQTTClient.h>
#include "../types.h"
#include "../controller/DoorController.h"
#include "../GarageDoorSytem.h"
#include "lib/Mqtt_tool.h"

class GarageDoorSystem;

class MQTTHandler_t : public Mqtt_tool {
private:
    std::string COMMAND_TOPIC = "garage/door/command";
    std::string STATUS_TOPIC = "garage/door/status";
    std::string ERROR_TOPIC = "garage/door/error";
    // DoorController_t &controller;
    // GarageDoorSystem &system;
    static MQTTHandler_t* currentHandler;

public:
    // MQTTHandler_t( DoorController_t &controller, GarageDoorSystem &system,
    //               const std::string &ssid, const std::string &pw,
    //               const string &hostname, int port, const string &clientID);

    MQTTHandler_t(const std::string &ssid, const std::string &pw,
        const string &hostname, int port, const string &clientID);



    DoorController_t& getController();

    GarageDoorSystem& getSystem();

    static void messageHandler(MQTT::MessageData &md);

    void processMessage(MQTT::MessageData &md);

};

#endif //MQTTHANDLER_H


#if 0

#endif
