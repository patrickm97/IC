#ifndef CONFIGMQTT_HPP
#define CONFIGMQTT_HPP

//#include "localstorage.hpp"
//#include "wificonnector.hpp"
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

        void setInterpretConfigCallback(const Mqtt::interpretConfigCallback& interpretCallback){
            this->mqtt.setInterpretConfigCallback(interpretCallback);
        }
        
        void connectMqtt(const char* ssid, const char* password, const char* topic, const char* mqttHost , uint16_t socket, const char* deviceId) {
            mqtt.connectMQTT(ssid, password, topic, mqttHost, socket, deviceId);
        }

        void publish(String sensor, String value) {
            mqtt.publish(sensor, value);
        }

        void loopMqtt() {
            mqtt.loopMqtt();
        }
};

#endif