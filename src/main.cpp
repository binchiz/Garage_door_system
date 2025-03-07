#include <cstdio>
#include <cstring>
#include <cmath>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/timer.h"

#include "MQTTClient.h"
#include "./handler/lib/Mqtt_tool.h"
#include "Countdown.h"
#include "IPStack.h"
#include "handler/MQTTHandler.h"

class DoorController_t;
class GarageDoorSystem;

int main() {
    stdio_init_all();

    // 创建MQTT处理器
    MQTTHandler_t mqtt_handler{
        "Redmi_138D",     // WiFi SSID
        "zzyzmy20272025888", // WiFi 密码
        "192.168.31.12",    // MQTT 服务器地址
        2883,               // MQTT 服务器端口
        "GarageDoor-Client" // 客户端ID
    };

    string message = "garage/door/status, hello,world";
    mqtt_handler.publish_MQTT(MQTT::QOS1, "garage/door/status", message.data(), message.size());


    while (true) {
        //assume 50s
        int count = 10;
        int rc = mqtt_handler.yield_MQTT(2000);
        if (rc != 0) {
            printf("rc from yield is %d\n", rc);
            printf("Reconnect to MQTT broker \n");
            mqtt_handler.connect_MQTT();
            mqtt_handler.subscribe_MQTT("garage/door/command", MQTT::QOS1);
        }
        while (count > 0) {
            printf("count: %d ", count);
            count--;
            sleep_ms(5000);
        }
        printf("\n");
    }
}
