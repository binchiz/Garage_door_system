//
// Created by zbinc on 23/02/2025.
//

#include "MQTTHandler.h"
#include <iostream>

MQTTHandler_t* MQTTHandler_t::currentHandler = nullptr;  // 在cpp文件中定义

//
// MQTTHandler_t::MQTTHandler_t(DoorController_t &controller, GarageDoorSystem &system,
//                              const std::string &ssid, const std::string &pw,
//                              const string &hostname, int port, const string &clientID)
//     : controller(controller), system(system), Mqtt_tool(ssid, pw, hostname, port, clientID) {
//     connect_MQTT();
//     subscribe_MQTT(COMMAND_TOPIC, MQTT::QOS1);
//     subscribe_MQTT(STATUS_TOPIC, MQTT::QOS1);
//     subscribe_MQTT(ERROR_TOPIC, MQTT::QOS1);
//     currentHandler = this;
// }


MQTTHandler_t::MQTTHandler_t(
                             const std::string &ssid, const std::string &pw,
                             const string &hostname, int port, const string &clientID)
    : Mqtt_tool(ssid, pw, hostname, port, clientID) {
    connect_MQTT();
    subscribe_MQTT(COMMAND_TOPIC, MQTT::QOS1);
    subscribe_MQTT(STATUS_TOPIC, MQTT::QOS1);
    subscribe_MQTT(ERROR_TOPIC, MQTT::QOS1);
    currentHandler = this;
}

// DoorController_t& MQTTHandler_t::getController() {
//     return controller;
// }
//
// GarageDoorSystem& MQTTHandler_t::getSystem() {
//     return system;
// }

void MQTTHandler_t::messageHandler(MQTT::MessageData &md) {
    if (currentHandler) {
        currentHandler->processMessage(md);
    }
}

void MQTTHandler_t::processMessage(MQTT::MessageData &md) {
    MQTT::Message &message = md.message;
    std::vector<char> temp_buffer(message.payloadlen + 1, '\0');
    memcpy(temp_buffer.data(), message.payload, message.payloadlen);
    std::string mess = temp_buffer.data();
    std::cout << "Message received [length: " << message.payloadlen
            << "]: '" << temp_buffer.data() << "'" << std::endl;

    // Status_t status = controller.getDoorStatus();
    // if(status.calibState == UNCALIBRATED) {
    //     system.addCommand(CALIB);
    //     return;
    // }
    // system.addCommand(CALIB);
}



// MQTTHandler_t::MQTTHandler_t(const std::string &ssid, const std::string &pw, const string &hostname, int port,
//     const string &clientID) : Mqtt_tool(ssid, pw, hostname, port, clientID) {
//     connect_MQTT();
//     subscribe_MQTT(COMMAND_TOPIC, MQTT::QOS1);
//     subscribe_MQTT(STATUS_TOPIC, MQTT::QOS1);
//     subscribe_MQTT(ERROR_TOPIC, MQTT::QOS1);
// }

