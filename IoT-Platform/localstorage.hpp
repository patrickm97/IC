#ifndef LOCALSTORAGE_HPP
#define LOCALSTORAGE_HPP

#include <iostream>
#include <Arduino.h>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

class LocalStorage {
    //template <typename T>
    private:
        vector<string> deviceIds;

    public:
        // initializing with sample
        LocalStorage() {
            this->deviceIds.push_back("esp32");
        }

        void saveData(string deviceId) {
            // check if it is already saved
            if(this->isDeviceSaved(deviceId)) {
                Serial.println("Error, device was already saved...");
                return;
            }

            // add new deviceId
            this->deviceIds.push_back(deviceId);

            // print list
            this->printListOfDevices();
        }

        void printListOfDevices() {
            int n = 0;
            for (int i = 0; i < this->deviceIds.size(); i++) {
                n = deviceIds[i].length();
                char char_array[n+1];
                strcpy(char_array, deviceIds[i].c_str()); // c_str: convert string to const char *
                Serial.print("DeviceId element ");
                Serial.print(i);
                Serial.print(": ");
                Serial.println(char_array);
            }
        }

        bool isDeviceSaved(string deviceId) {
            bool found = (find(deviceIds.begin(), deviceIds.end(), deviceId) != deviceIds.end());
            return found;
        }

        string loadData(string deviceId) {
            vector<string>::iterator iterate = find(this->deviceIds.begin(), this->deviceIds.end(), deviceId);
            
            if (iterate == this->deviceIds.end()) {
                Serial.println("Error, data not found");
                return "";
            }
            // data found, get index and return
            int index = distance(this->deviceIds.begin(), iterate);
            return *iterate;
            //return this->deviceIds[index];
        }
};

#endif