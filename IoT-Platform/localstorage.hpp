#ifndef LOCALSTORAGE_HPP
#define LOCALSTORAGE_HPP

#include <iostream>
#include <Arduino.h>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iterator>
#include <EEPROM.h>
using namespace std;

#define MAIN_POSITION_CONFIGURED 0
#define START_POSITION_DEVICEID 1
#define START_POSITION_SSID 13
#define START_POSITION_PASS 50
#define LENGTH_LIMIT_DEVICEID 10
#define LENGTH_LIMIT_SSID 20
#define LENGTH_LIMIT_PASS 15

class LocalStorage {
    //template <typename T>
    private:
        vector<string> deviceIds;

    public:
        /* initializing with sample
        LocalStorage() {
            this->deviceIds.push_back("esp32");
        }*/

        void saveData(const string deviceId, const string ssid, const string pass) {
            // check if it is already saved
            if(this->isDeviceConfigured()) {
                Serial.println("Reset option, coming soon...");
                return;
            }

            // add data to vector
            this->deviceIds.push_back(deviceId);
            this->deviceIds.push_back(ssid);
            this->deviceIds.push_back(pass);
            
            /* reserved addresses in flash memory:
            *  deviceId: address 1 + length (length limit of 10) + /0 (1 to 12)
            *  ssid: address 13 + length (length limit of 20) + /0 (13 to 49)
            *  pass: address 50 + length (length limit of 15) + /0 (50 to 65)
            */
            int deviceIdLen = deviceId.length();
            int ssidLen = ssid.length();
            int passLen = pass.length();

            if (deviceIdLen == 0 || ssidLen == 0 || passLen == 0) {
                Serial.println("Exception, deviceId, ssid and password cannot be empty");
                return;
            }

            // save deviceId in flash memory (1 to 12)
            if (deviceIdLen > LENGTH_LIMIT_DEVICEID) {
                Serial.println("Exception, deviceId cannot be greater than 10");
                return;
            }

            for (int i = START_POSITION_DEVICEID; i < deviceIdLen + 1; i++) {
                if (i == deviceIdLen) // end character in last position
                    EEPROM.write(i, '\0');
                else
                    EEPROM.write(i, deviceId.c_str()[i]);
            }

            // save ssid in flash memory (13 to 49)
            if (ssidLen > LENGTH_LIMIT_SSID) {
                Serial.println("Exception, ssid cannot be greater than 10");
                return;
            }

            for (int i = START_POSITION_SSID; i < ssidLen + 1; i++) {
                if (i == ssidLen) // end character in last position
                    EEPROM.write(i, '\0');
                else
                    EEPROM.write(i, ssid[i]);
            }

            // save password in flash memory (50 to 65)
            if (passLen > LENGTH_LIMIT_PASS) {
                Serial.println("Exception, password cannot be greater than 15");
                return;
            }

            for (int i = START_POSITION_PASS; i < passLen + 1; i++) {
                if (i == passLen) // end character in last position
                    EEPROM.write(i, '\0');
                else
                    EEPROM.write(i, pass[i]);
            }

            EEPROM.write(MAIN_POSITION_CONFIGURED, 1);

            // print list
            this->printDeviceAndNetworkInfo();
        }

        void printDeviceAndNetworkInfo() {
            int n = 0, value = 0;
            for (int i = 1; i < 65; i++) {
                /*
                n = deviceIds[i].length();
                char char_array[n+1];
                strcpy(char_array, deviceIds[i].c_str()); // c_str: convert string to const char *
                Serial.print("DeviceId element ");
                Serial.print(i);
                Serial.print(": ");
                Serial.println(char_array);
                */
                
                if (i == START_POSITION_DEVICEID)
                    Serial.print("Device ID: ");
                if (i == START_POSITION_SSID)
                    Serial.print("SSID: ");
                if (i == START_POSITION_PASS)
                    Serial.print("Password: ");

                value = EEPROM.read(i);
                Serial.print(value);

                if (i == 12 || i == 49 || i == 65) // line break
                    Serial.println("");
            }
            Serial.println("");
        }
        /*
        *  checks if device is currently configured
        *  returns true if there is a 1 stored at flash position 0
        */
        bool isDeviceConfigured() {
            //bool found = (find(deviceIds.begin(), deviceIds.end(), deviceId) != deviceIds.end());
            //return found;
            if (EEPROM.read(MAIN_POSITION_CONFIGURED) == 1)
                return true;
            return false;
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