#ifndef WEBCONNECTOR_HPP
#define WEBCONNECTOR_HPP

#include "wifiserverconfigurator.hpp"
using namespace std;

class WebConnector {
    private:
        LocalStorage &storage; // & = reference to object
        WifiServerConfigurator wifiServer; // don't use & when class owns this object
        
    public:
        WebConnector(LocalStorage &storage) : storage(storage), wifiServer(storage) {
        }

        bool isDeviceConfigured() {
            return this->wifiServer.isDeviceConfigured();
        }

        void saveData(String deviceId, String ssid, String password, String mqttHost, 
                      String mqttPass, String topic) {
            this->wifiServer.saveData(deviceId, ssid, password, mqttHost, mqttPass, topic);
        }

        String loadData(bool mqttPassExists) {
            return this->wifiServer.loadData(mqttPassExists);
        }
};

#endif