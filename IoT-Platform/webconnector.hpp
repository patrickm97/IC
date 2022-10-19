#ifndef WEBCONNECTOR_HPP
#define WEBCONNECTOR_HPP

#include "wifiserverconfigurator.hpp"
#include "wificonnector.hpp"

using namespace std;

class WebConnector {
    private:
        LocalStorage &storage; // & = reference to object
        WifiServerConfigurator wifiServerConfigurator; // don't use & when class owns this object
        WifiConnector &wifiConnector;
        
    public:
        WebConnector(WifiConnector &wifiConnector, LocalStorage &storage) : wifiConnector(wifiConnector), storage(storage), wifiServerConfigurator(storage) {
        }

        bool isDeviceConfigured() {
            return this->wifiServerConfigurator.isDeviceConfigured();
        }

        void saveData(String deviceId, String ssid, String password, String mqttHost, 
                      String mqttPass, String topic) {
            this->wifiServerConfigurator.saveData(deviceId, ssid, password, mqttHost, mqttPass, topic);
        }

        String loadData(bool mqttPassExists) {
            return this->wifiServerConfigurator.loadData(mqttPassExists);
        }

        void setupWebServer() {
            wifiConnector.setupWebServer();
        }
};

#endif