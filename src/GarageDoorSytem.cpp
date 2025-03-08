
#include "GarageDoorSytem.h"

#include <iostream>
#include <ostream>

GarageDoorSystem::GarageDoorSystem(
    DoorController_t& doorController,
    ButtonHandler_t& buttonHandler,
    MQTTHandler_t& mqttHandler)
{
    this->doorController = &doorController;
    this->buttonHandler = &buttonHandler;
    this->mqttHandler = &mqttHandler;
}

void GarageDoorSystem::initialize(DoorController_t& doorController,
                                ButtonHandler_t& buttonHandler,
                                MQTTHandler_t& mqttHandler) {
    this->doorController = &doorController;
    this->buttonHandler = &buttonHandler;
    this->mqttHandler = &mqttHandler;
}

void GarageDoorSystem::update() {
    buttonHandler->update();
    mqttHandler->yield_MQTT(100);
}


void GarageDoorSystem::addCommand(const command c) {
    commandQueue.push(c);
}

void GarageDoorSystem::doorOpening() {
    bool opening = true;
    while (opening) {
        doorController->open();
        if (!doorController->isMoving()) opening = false;
        update();
        if (!commandQueue.empty()) {
            commandQueue.pop();
            opening = false;
        }
    }
}

void GarageDoorSystem::doorClosing() {
    bool closing = true;
    while (closing) {
        doorController->close();
        if (!doorController->isMoving()) closing = false;
        update();
        if (!commandQueue.empty()) {
            commandQueue.pop();
            closing = false;
        }
    }
}

void GarageDoorSystem::run() {
        update();
        if (!commandQueue.empty()) {
            command command = commandQueue.front();
            commandQueue.pop();
            switch (command) {
                case OPEN:
                    if (doorController->isCalibrated()) {
                        doorOpening();
                    }
                    break;
                case CLOSE:
                    if (doorController->isCalibrated()) {
                        doorClosing();
                    }
                    break;
                case STOP:
                    doorController->stop();
                    break;
                case CALIB:
                    doorController->calibrate();
                    break;
            }

        }
    }







