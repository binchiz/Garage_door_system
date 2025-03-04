#include "Mqtt_tool.h"

#include "../MQTTHandler.h"


Mqtt_tool::Mqtt_tool(const string &ssid, const string &pw, const string &hostname, int port, const string &clientID)
    : ipstack(ssid.c_str(), pw.c_str()), client(ipstack), hostname(hostname), port(port), clientID(clientID) {
}

void Mqtt_tool::connect_MQTT() {
    int rc = ipstack.connect(hostname.data(), port);
    if (rc != 1) {
        printf("rc from TCP connect is %d\n", rc);
    }
    printf("MQTT connecting\n");
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = const_cast<char *>(clientID.data());
    rc = client.connect(data);
    if (rc != 0) {
        printf("rc from MQTT connect is %d\n", rc);
    }
    printf("MQTT connected\n");
}

void Mqtt_tool::subscribe_MQTT(const string &mqtt_topic, MQTT::QoS qos_level ) {
    int rc = client.subscribe(mqtt_topic.data(), qos_level, MQTTHandler_t::messageHandler);
    if (rc != 0) {
        printf("rc from MQTT subscribe is %d\n", rc);
    }
    printf("MQTT subscribe is successful\n");
}

void Mqtt_tool::publish_MQTT(MQTT::QoS qos_level ,std::string mqtt_topic, void *mqtt_message, int mqtt_message_len) {
    MQTT::Message message;  // 使用默认构造函数（无参数）
    message.qos = qos_level;
    message.retained = 1;
    message.dup = false;
    message.id = 0;
    message.payload = mqtt_message;
    message.payloadlen = mqtt_message_len;
    int rc = client.publish(mqtt_topic.data(), message);
    if (rc != 0) {
        printf("rc from MQTT publish is %d\n", rc);
    }
    printf("send message: %s\n", (char *) mqtt_message);
}

int Mqtt_tool::yield_MQTT(int yield_time) {
    return client.yield(yield_time);
}




