
#include "GarageDoorSytem.h"

GarageDoorSystem::GarageDoorSystem(
    DoorController_t &doorController,
    Storage_t &Storage,
    ButtonHandler_t &ButtonHandler,
    MQTTHandler_t &MQTTHandler)
    : doorController(doorController),storage(Storage), buttonHandler(buttonHandler), mqttHandler(mqttHandler)
{

}

void GarageDoorSystem::addCommand(const command c) {
    commandQueue.push(c);
}





