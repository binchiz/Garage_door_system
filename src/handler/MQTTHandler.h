/*handles mqtt stuff
 *1. handles remote command
 *2. report status
 */
#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H
//#include <mosquitto.h>
#include "types.h"
#include "controller/DoorController.h"
#include "GarageDoorSytem.h"
#include <MQTTClient.h>
#include "lib/Mqtt_tool.h"

class DoorController_t;
class GarageDoorSystem;

// #define ctrl
#define no_ctrl

class MQTTHandler_t : public Mqtt_tool {
private:
    std::string COMMAND_TOPIC = "garage/door/command";
    std::string STATUS_TOPIC = "garage/door/status";
    std::string ERROR_TOPIC = "garage/door/error";

#ifdef ctrl
    DoorController_t &controller;
    GarageDoorSystem &system;
#endif
    static std::unique_ptr<MQTTHandler_t> currentHandler;

public:
#ifdef ctrl
    MQTTHandler_t( DoorController_t &controller, GarageDoorSystem &system,
                  const std::string &ssid, const std::string &pw,
                  const string &hostname, int port, const string &clientID);

    DoorController_t& getController();

    GarageDoorSystem& getSystem();
#endif

#ifdef no_ctrl
    void subscribe_all();

    MQTTHandler_t(const std::string &ssid, const std::string &pw,
                  const string &hostname, int port, const string &clientID);
#endif

    static void messageHandler(MQTT::MessageData &md);

    void processMessage(MQTT::MessageData &md);

};

#endif //MQTTHANDLER_H


#if 0

#endif
