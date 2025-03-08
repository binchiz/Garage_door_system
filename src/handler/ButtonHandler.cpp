
#include "ButtonHandler.h"

ButtonHandler_t::ButtonHandler_t(
    DoorController_t &doorController,
    Button_t &sw0,
    Button_t &sw1,
    Button_t &sw2,
    GarageDoorSystem &system) : doorController(doorController), sw0(sw0), sw1(sw1), sw2(sw2), system(system) {

}

