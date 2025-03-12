#include "pico/stdlib.h"
#include "./handler/lib/Mqtt_tool.h"
#include "hardware/Button.h"
#include "hardware/RotaryEncoder.h"
#include "handler/GPIOInterrupt.h"
#include "hardware/EEPROM.h"
#include "hardware/StepperMotor.h"
#include "hardware/LED.h"

#include "handler/ButtonHandler.h"
#include "handler/MQTTHandler.h"
#include "controller/Storage.h"
#include "controller/DoorController.h"
#include "GarageDoorSytem.h"

using namespace MQTTTMessage;

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
    LED_t leds(20, 21, 22);

    EEPROM_t eeprom;
    DoorController_t controller(upper, lower, encoder, motor, leds);
    Storage_t storage(eeprom, controller);

    GarageDoorSystem doorSystem;

    ButtonHandler_t buttonHandler(controller, sw0, sw1, sw2, doorSystem);
    MQTTHandler_t mqtt_handler{
        controller,
        doorSystem,
        "",
        "",
        "192.168.1.107",
        2883,
        "GarageDoor-Client"
    };

    controller.setButtonHandler(&buttonHandler);
    controller.setMQTTHandler(&mqtt_handler);
    doorSystem.initialize(controller, buttonHandler, mqtt_handler, storage);

    doorSystem.loadStoredStatus();
    mqtt_handler.publish_MQTT(MQTT::QOS1, RESPONSE_TOPIC, const_cast<void*>(static_cast<const void*>(REBOOT_MSG)), strlen(REBOOT_MSG));
    doorSystem.reportStatus();

    while (true) {
        doorSystem.run();
    }
}
