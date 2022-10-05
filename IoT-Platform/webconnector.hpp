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

        bool isDeviceConfigured(string deviceId) {
            if (this->wifiServer.isDeviceConfigured(deviceId)) {
                return true;
            }
            wifiServer.configureServer(deviceId);
        }

        void configureServer(string deviceId) {
            this->wifiServer.configureServer(deviceId);
        }

        string loadData(string devideId) {
            return wifiServer.loadData(devideId);
        }
};

#endif