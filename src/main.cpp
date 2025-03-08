#include <cstdio>
#include <cstring>
#include <cmath>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/timer.h"

#include "./handler/lib/Mqtt_tool.h"
#include "hardware/Button.h"
#include "hardware/RotaryEncoder.h"
#include "hardware/EEPROM.h"
#include "hardware/StepperMotor.h"

#include "handler/ButtonHandler.h"
#include "handler/MQTTHandler.h"
#include "controller/Storage.h"
#include "controller/DoorController.h"
#include "GarageDoorSytem.h"



int main() {
    Button_t sw0(9);
    Button_t sw1(8);
    Button_t sw2(7);
    LimitSwitch_t upper(16);
    LimitSwitch_t lower(17);
    StepperMotor_t motor(2, 3, 6, 13);
    RotaryEncoder_t encoder(28,27);
    DoorController_t controller(upper, lower, encoder, motor);
    ButtonHandler_t buttonHandler(controller, sw0, sw1, sw2, system);
    MQTTHandler_t mqtt_handler{
        "ssid",     // WiFi SSID
        "pwd", // WiFi 密码
        "192.168.1.107",    // MQTT 服务器地址
        2883,               // MQTT 服务器端口
        "GarageDoor-Client" // 客户端ID
    };
    Storage_t storage;
    GarageDoorSystem system(controller, storage, buttonHandler, mqtt_handler);
}
