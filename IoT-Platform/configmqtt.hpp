#ifndef CONFIGMQTT_HPP
#define CONFIGMQTT_HPP

#include "localstorage.hpp"
#include "wificlient.hpp"
#include "mqtt.hpp"

class ConfigMqtt {
    private:
        Mqtt mqtt;
        LocalStorage &storage;
        WifiClient &wifiClient;

    public:
        ConfigMqtt(WifiClient &wifiClient, LocalStorage &storage) : 
        wifiClient(wifiClient),
        mqtt(wifiClient), 
        storage(storage) {  
        }
    
};

#endif