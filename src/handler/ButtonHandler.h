
#ifndef BOTTONHANDLER_H
#define BOTTONHANDLER_H
#include "controller/DoorController.h"
#include "hardware/Button.h"
#include "types.h"
#include "GarageDoorSytem.h"


class GarageDoorSystem;

class ButtonHandler_t{
    public:
        ButtonHandler_t(
            DoorController_t& doorController,
            Button_t& sw0,
            Button_t& sw1,
            Button_t& sw2,
            GarageDoorSystem& system
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
        Button_t& sw0;
        Button_t& sw1;
        Button_t& sw2;
        DoorController_t& doorController;
        GarageDoorSystem& system;
};

#endif //BOTTONHANDLER_H
