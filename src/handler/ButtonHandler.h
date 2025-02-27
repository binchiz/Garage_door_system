
#ifndef BOTTONHANDLER_H
#define BOTTONHANDLER_H
#include "../controller/DoorController.h"
#include "../hardware/Button.h"
#include "../types.h"
#include "../GarageDoorSytem.h"

#include <memory>

class ButtonHandler {
    public:
        ButtonHandler(
            DoorController *doorController,
            std::unique_ptr<Button>sw0,
            std::unique_ptr<Button>sw1,
            std::unique_ptr<Button>sw2
            //...
            );
        command createCommandFromInput();
        /*generating the correct command according to controller status
         * if sw0 and sw2 pressed at same time for 2 second -> calib
         * if sw1 pressed:
         *  door is currently opening or closing -> stop
         *  opened -> close
         *  closed -> open
         *  stopped -> go to opposite direction
         *  stuck -> stop
         * ...
         */
        void update(); // call system.addCommand()
    private:
        std::unique_ptr<Button> sw0;
        std::unique_ptr<Button> sw1;
        std::unique_ptr<Button> sw2;
        DoorController& doorController;
        GarageDoorSystem& system;
};

#endif //BOTTONHANDLER_H
