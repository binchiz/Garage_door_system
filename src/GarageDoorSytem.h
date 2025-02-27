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

class GarageDoorSystem {
    public:
        GarageDoorSystem();
        void initSystem(); //call component's init methods
        void addCommand(); //push command to queue. used by handlers
        void run();
    private:
        DoorController& DoorController;
        Storage& Storage;
        ButtonHandler& ButtonHandler;
        MQTTHandler& MQTTHandler;
        std::queue<command> commandQueue;
};

#endif //GARAGEDOORSYTEM_H
