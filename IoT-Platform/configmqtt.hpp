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

        void displayWifiNetworks() {
            mqtt.displayWifiNetworks();
        }
    
        void connectMqtt(const char* ssid, const char* password, const char* topic, const char* mqttHost , int socket, const char* mqttPass) {
            Serial.print("ssid in configmqtt: ");
            Serial.print(ssid);
            Serial.print(", password in configmqtt: ");
            Serial.println(password);
            mqtt.connectMQTT(ssid, password, topic, mqttHost, socket, mqttPass);
        }

        void subscribe() {
            mqtt.subscribe();
        }

        void publish() {
            mqtt.publish();
        }
};

#endif