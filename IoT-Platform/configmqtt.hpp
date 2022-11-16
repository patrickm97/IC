#ifndef CONFIGMQTT_HPP
#define CONFIGMQTT_HPP

#include "localstorage.hpp"
#include "wificonnector.hpp"
#include "mqtt.hpp"

class ConfigMqtt {
    private:
        Mqtt mqtt;
        LocalStorage &storage;
        WifiConnector &wifiConnector;

    public:
        ConfigMqtt(WifiConnector &wifiConnector, LocalStorage &storage) : 
        wifiConnector(wifiConnector), mqtt(wifiConnector), storage(storage) {  
        }
        /*
        void displayWifiNetworks() {
            mqtt.displayWifiNetworks();
        }
        */
        void connectMqtt(const char* ssid, const char* password, const char* topic, const char* mqttHost , uint16_t socket, const char* deviceId) {
            Serial.print("topic in configmqtt: ");
            Serial.print(topic);
            Serial.print(", deviceId in configmqtt: ");
            Serial.println(deviceId);
            mqtt.connectMQTT(ssid, password, topic, mqttHost, socket, deviceId);
        }

        void subscribe() {
            //mqtt.subscribe();
        }

        void publish(String sensor, String value) {
            mqtt.publish(sensor, value);
        }

        void loopMqtt() {
            mqtt.loopMqtt();
        }
};

#endif