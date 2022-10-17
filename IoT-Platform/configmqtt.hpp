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
        char topic[50] = "dev";

    public:
        ConfigMqtt(WifiConnector &wifiConnector, LocalStorage &storage, const char* ssid, const char* pass, char* ) : 
        wifiConnector(wifiConnector), mqtt(wifiConnector), storage(storage) {  
        }

        void displayWifiNetworks() {
            mqtt.displayWifiNetworks();
        }
    
        void connectMqtt(const char* ssid, const char* password, char* topic, const uint16_t socket)
        {
            mqtt.connectMQTT(ssid, password, topic, socket);
        }
};

#endif