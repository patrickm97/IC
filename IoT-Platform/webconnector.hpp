#ifndef WEBCONNECTOR_HPP
#define WEBCONNECTOR_HPP

#include <string>
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

        void configureServer(const string deviceId, const string ssid, const string pass) {
            this->wifiServer.configureServer(deviceId, ssid, pass);
        }

        string loadData(string devideId) {
            return wifiServer.loadData(devideId);
        }
};

#endif