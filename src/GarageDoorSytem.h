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
        GarageDoorSystem(DoorController_t& doorController, ButtonHandler_t& ButtonHandler, MQTTHandler_t& MQTTHandler, Storage_t& storage);
        GarageDoorSystem() = default;
        ~GarageDoorSystem() = default;
        void initialize(DoorController_t& doorController,
               ButtonHandler_t& buttonHandler,
               MQTTHandler_t& mqttHandler,
               Storage_t& storage);
        void loadStoredStatus() const;
        void saveStatus() const;
        void restore();
        void update(); //update commands
        void addCommand(command c); //push command to queue. used by handlers
        void sendResponse();
        void reportStatus();
        void run();

    private:
        DoorController_t* doorController = nullptr;
        ButtonHandler_t* buttonHandler = nullptr;
        MQTTHandler_t* mqttHandler = nullptr;
        Storage_t* storage = nullptr;
        std::queue<command> commandQueue;
        uint32_t moveStartTime;
        static constexpr uint32_t STUCK_TIMEOUT = 1000;

};

#endif //GARAGEDOORSYTEM_H
