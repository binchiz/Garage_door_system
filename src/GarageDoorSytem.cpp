
#include "GarageDoorSytem.h"
#include "types.h"

#include <iostream>

using namespace MQTTTMessage;

GarageDoorSystem::GarageDoorSystem(
    DoorController_t& doorController,
    ButtonHandler_t& buttonHandler,
    MQTTHandler_t& mqttHandler,
    Storage_t& storage)
{
    this->doorController = &doorController;
    this->buttonHandler = &buttonHandler;
    this->mqttHandler = &mqttHandler;
    this->storage = &storage;
}

void GarageDoorSystem::initialize(DoorController_t& doorController,
                                ButtonHandler_t& buttonHandler,
                                MQTTHandler_t& mqttHandler,
                                Storage_t& storage) {
    this->doorController = &doorController;
    this->buttonHandler = &buttonHandler;
    this->mqttHandler = &mqttHandler;
    this->storage = &storage;
}

void GarageDoorSystem::saveStatus() const {
    storage->saveCalib();
    storage->saveError();
    storage->saveState();
    storage->saveTotalSteps();
}


void GarageDoorSystem::loadStoredStatus() const {
    storage->loadCalib();
    storage->loadError();
    storage->loadState();
    storage->loadTotalSteps();
}


void GarageDoorSystem::update() {
    buttonHandler->update();
    mqttHandler->yield_MQTT(100);
}


void GarageDoorSystem::addCommand(const command c) {
    commandQueue.push(c);
}


void GarageDoorSystem::sendResponse() const {
    if (doorController->isStuck()) mqttHandler->publish_MQTT(MQTT::QOS1, RESPONSE_TOPIC, const_cast<void*>(static_cast<const void*>(STUCK_MSG)), strlen(STUCK_MSG));
    if (!doorController->isCalibrated()) mqttHandler->publish_MQTT(MQTT::QOS1, RESPONSE_TOPIC, const_cast<void*>(static_cast<const void*>(NO_CALIB_MSG)), strlen(NO_CALIB_MSG));
    else mqttHandler->publish_MQTT(MQTT::QOS1, RESPONSE_TOPIC, const_cast<void*>(static_cast<const void*>(SUCCESS_MSG)), strlen(SUCCESS_MSG));
}


void GarageDoorSystem::reportStatus() const {

    std::string statusMsg = "Door:";
    statusMsg += GarageDoor::getDoorStateString(doorController->getDoorStatus());
    statusMsg += ",Error:";
    statusMsg += getErrorStateString(doorController->isStuck());
    statusMsg += ",Calib:";
    statusMsg += getCalibStateString(doorController->isCalibrated());

    mqttHandler->publish_MQTT(MQTT::QOS1, STATUS_TOPIC, statusMsg.data(), statusMsg.size());

}

void GarageDoorSystem::run() {
    doorController->controlLed();
    update();
    if (!commandQueue.empty()) {
        command command = commandQueue.front();
        commandQueue.pop();
        switch (command) {
        case OPEN:
            if (doorController->isCalibrated()&(doorController->getDoorStatus()!=GarageDoor::OPENED)) {
                doorController->setDoorStatus(GarageDoor::OPENING);
                saveStatus();
                doorController->open();
            }
            sendResponse();
            saveStatus();
            reportStatus();
            break;
        case CLOSE:
            if (doorController->isCalibrated()&doorController->getDoorStatus()!=GarageDoor::CLOSED) {
                doorController->setDoorStatus(GarageDoor::CLOSING);
                saveStatus();
                doorController->close();
            }
            sendResponse();
            saveStatus();
            reportStatus();
            break;
        case STOP:
            doorController->stop();
            break;
        case CALIB:
            doorController->calibrate();
            sendResponse();
            saveStatus();
            reportStatus();
            break;
            }
        }
    }







