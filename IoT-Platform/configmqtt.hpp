#ifndef CONFIGMQTT_HPP
#define CONFIGMQTT_HPP

#include "localstorage.hpp"
#include "wificonnector.hpp""
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
    
        void connectMqtt(const char* ssid, const char* password, char* topic, char* mqttHost , int socket, char* mqttPass)
        {
            mqtt.connectMQTT(ssid, password, topic, mqttHost, socket, mqttPass);
        }
};

#endif