#include <cstdio>
#include <cstring>
#include <cmath>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/timer.h"

#include "./handler/lib/Mqtt_tool.h"
#include "hardware/Button.h"
#include "hardware/RotaryEncoder.h"
#include "handler/GPIOInterrupt.h"
#include "hardware/EEPROM.h"
#include "hardware/StepperMotor.h"

#include "handler/ButtonHandler.h"
#include "handler/MQTTHandler.h"
#include "controller/Storage.h"
#include "controller/DoorController.h"
#include "GarageDoorSytem.h"

#include <iostream>


int main() {
    stdio_init_all();
    GPIOInterrupt::init();


    Button_t sw0(9, true);
    Button_t sw1(8, true);
    Button_t sw2(7, true);
    LimitSwitch_t upper(16, true);
    LimitSwitch_t lower(17, true);
    StepperMotor_t motor(2, 3, 6, 13);
    RotaryEncoder_t encoder(28,27, true);

    DoorController_t controller(upper, lower, encoder, motor);

    GarageDoorSystem doorSystem;

    ButtonHandler_t buttonHandler(controller, sw0, sw1, sw2, doorSystem);
    MQTTHandler_t mqtt_handler{
        controller,
        doorSystem,
        "B38-2G",     // WiFi SSID
        "[pwd]", // WiFi 密码
        "192.168.1.107",    // MQTT 服务器地址
        2883,               // MQTT 服务器端口
        "GarageDoor-Client" // 客户端ID
    };

    doorSystem.initialize(controller, buttonHandler, mqtt_handler);


    std::cout << "initialization complete" << std::endl;
    while (true) {
        doorSystem.run();
    }


}
