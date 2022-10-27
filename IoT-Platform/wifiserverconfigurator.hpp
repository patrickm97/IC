#ifndef WIFISERVERCONFIGURATOR_HPP
#define WIFISERVERCONFIGURATOR_HPP

#include "localstorage.hpp"
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

class WifiServerConfigurator {
    private:
        LocalStorage &storage;
        
    public:
        WifiServerConfigurator(LocalStorage &storage): storage(storage) {
        }

        bool isDeviceConfigured() {
            return this->storage.isDeviceConfigured();
        }

        void saveData(String deviceId, String ssid, String password, String mqttHost, 
                      String mqttPass, String topic) {
            this->storage.saveData(deviceId, ssid, password, mqttHost, mqttPass, topic);
        }

        String loadData() {
            return this->storage.loadData();
        }
};

#endif