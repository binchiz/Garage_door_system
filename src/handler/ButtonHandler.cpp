
#include "ButtonHandler.h"
#include "types.h"

#include <iostream>

ButtonHandler_t::ButtonHandler_t(
    DoorController_t &doorController,
    Button_t &sw0,
    Button_t &sw1,
    Button_t &sw2,
    GarageDoorSystem &system) : doorController(doorController), sw0(sw0), sw1(sw1), sw2(sw2), system(system) {

}

command ButtonHandler_t::createCommandFromInput() const {
    if ((sw0.isPressed() && sw2.isPressing()) || (sw2.isPressed() && sw0.isPressing())) {
        std::cout<< "getting calib command" << std::endl;
        return CALIB;
    }
    if (sw1.isPressed()) {
        std::cout << "local button pressed" << std::endl;
        const GarageDoor::doorState ds = doorController.getDoorStatus();
        if (doorController.isMoving()) {
            std::cout << "stop command" << std::endl;
            return STOP;
        }
        switch (ds) {
            case GarageDoor::CLOSED:
                return OPEN;
            case GarageDoor::OPENED:
                return CLOSE;
            case GarageDoor::CLOSING:
                return OPEN;
            case GarageDoor::OPENING:
                return CLOSE;
            default:
                return STOP;
        }
    }
    return STOP;
}

void ButtonHandler_t::update() const {
    const command c = createCommandFromInput();
    system.addCommand(c);
}



