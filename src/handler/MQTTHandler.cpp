
#include "MQTTHandler.h"
#include <algorithm>
#include <iostream>

namespace {
bool caseInsensitiveCompare(const std::string& str1, const std::string& str2) {
    std::string str1Copy = str1;
    std::string str2Copy = str2;

    std::transform(str1Copy.begin(), str1Copy.end(), str1Copy.begin(),
                  [](unsigned char c) { return std::tolower(c); });
    std::transform(str2Copy.begin(), str2Copy.end(), str2Copy.begin(),
                  [](unsigned char c) { return std::tolower(c); });
    return str1Copy == str2Copy;
}
}

std::unique_ptr<MQTTHandler_t> MQTTHandler_t::currentHandler;



// void MQTTHandler_t::subscribe_all() {
//     subscribe_MQTT(COMMAND_TOPIC, MQTT::QOS1);
//     subscribe_MQTT(STATUS_TOPIC, MQTT::QOS1);
//     subscribe_MQTT(ERROR_TOPIC, MQTT::QOS1);
//     subscribe_MQTT(RESPONSE_TOPIC, MQTT::QOS1);
// }

MQTTHandler_t::MQTTHandler_t(DoorController_t &controller, GarageDoorSystem &system,
                             const std::string &ssid, const std::string &pw,
                             const string &hostname, int port, const string &clientID)
    : Mqtt_tool(ssid, pw, hostname, port, clientID), doorController(controller), system(system){
    connect_MQTT();
    subscribe_MQTT(COMMAND_TOPIC, MQTT::QOS1);
    // subscribe_MQTT(STATUS_TOPIC, MQTT::QOS1);
    // subscribe_MQTT(ERROR_TOPIC, MQTT::QOS1);
    currentHandler.reset(this);
}

DoorController_t& MQTTHandler_t::getController() const {
    return doorController;
}

GarageDoorSystem& MQTTHandler_t::getSystem() {
    return system;
}


void MQTTHandler_t::messageHandler(MQTT::MessageData &md) {
    if (currentHandler) {
        currentHandler->processMessage(md);
    }
}

void MQTTHandler_t::processMessage(MQTT::MessageData &md) {
    MQTT::Message &message = md.message;
    std::vector<char> temp_buffer(message.payloadlen + 1, '\0');
    memcpy(temp_buffer.data(), message.payload, message.payloadlen);
    std::string m = temp_buffer.data(); // received message
    std::cout << "Message received [length: " << message.payloadlen
            << "]: '" << temp_buffer.data() << "'" << std::endl;

    if (caseInsensitiveCompare(m, "stop")) doorController.stop();
    if (caseInsensitiveCompare(m, "open")) system.addCommand(OPEN);
    if (caseInsensitiveCompare(m, "close")) system.addCommand(CLOSE);
    if (caseInsensitiveCompare(m, "calib")) system.addCommand(CALIB);

}








