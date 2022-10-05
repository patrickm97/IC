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

        bool isDeviceConfigured(string deviceId) {
            if (this->storage.isDeviceSaved(deviceId)) {
                return true;
            } 
            return false;
        }

        void configureServer(string deviceId) {
            this->storage.saveData(deviceId);
        }

        string loadData(string devideId) {
            return storage.loadData(devideId);
        }
};

#endif