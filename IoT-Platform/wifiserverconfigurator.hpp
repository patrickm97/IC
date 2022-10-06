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

        void configureServer(const string deviceId, const string ssid, const string pass) {
            this->storage.saveData(deviceId, ssid, pass);
        }

        string loadData(string devideId) {
            return storage.loadData(devideId);
        }
};

#endif