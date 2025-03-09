/*the main system
 * has a queue of commands, see type.h
 * while running:
 *  handlers push command to queue
 *  system fetch command from queue
 *  system call the corresponding controller methods
 *  system store status change to storage
 */
#ifndef GARAGEDOORSYTEM_H
#define GARAGEDOORSYTEM_H
#include "controller/DoorController.h"
#include "controller/Storage.h"
#include "handler/ButtonHandler.h"
#include "handler/MQTTHandler.h"
#include "types.h"

#include <queue>

class ButtonHandler_t;
class MQTTHandler_t;
class DoorController_t;
class Storage_t;


class GarageDoorSystem {

    public:
        GarageDoorSystem(DoorController_t& doorController, ButtonHandler_t& ButtonHandler, MQTTHandler_t& MQTTHandler);
        GarageDoorSystem() = default;
        ~GarageDoorSystem() = default;
        void initialize(DoorController_t& doorController,
               ButtonHandler_t& buttonHandler,
               MQTTHandler_t& mqttHandler);
        void loadStoredStatus();
        void saveStatus();
        void update();
        void addCommand(command c); //push command to queue. used by handlers
        void doorOpening();
        void doorClosing();
        void sendResponse();
        void run();

    private:
        DoorController_t* doorController = nullptr;
        ButtonHandler_t* buttonHandler = nullptr;
        MQTTHandler_t* mqttHandler = nullptr;
        std::queue<command> commandQueue;

};

#endif //GARAGEDOORSYTEM_H
