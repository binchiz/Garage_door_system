
#include "GarageDoorSytem.h"

GarageDoorSystem::GarageDoorSystem(
    DoorController_t &doorController,
    ButtonHandler_t &ButtonHandler,
    MQTTHandler_t &MQTTHandler)
    : doorController(doorController), buttonHandler(buttonHandler), mqttHandler(mqttHandler)
{

}

void GarageDoorSystem::update() {
    buttonHandler.update();
}


void GarageDoorSystem::addCommand(const command c) {
    commandQueue.push(c);
}





