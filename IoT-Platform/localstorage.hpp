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
// 255.255.255.255:1000 -> len = 20
#define START_POSITION_DEVICEID 1
#define START_POSITION_SSID 13
#define START_POSITION_PASS 35
#define START_POSITION_MQTTHOST 51
#define START_POSITION_MQTTPASS 72
#define START_POSITION_TOPIC 88

#define LENGTH_LIMIT_DEVICEID 10
#define LENGTH_LIMIT_SSID 20
#define LENGTH_LIMIT_PASS 15
#define LENGTH_LIMIT_MQTTHOST 20
#define LENGTH_LIMIT_MQTTPASS 15
#define LENGTH_LIMIT_TOPIC 10

class LocalStorage
{
private:
public:
    /* reserved addresses in flash memory:
    *  deviceId: address 1 + length (length limit of 10) + /0 (1 to 12)
    *  ssid: address 13 + length (length limit of 20) + /0 (13 to 49)
    *  pass: address 50 + length (length limit of 15) + /0 (50 to 65)
    *  mqttHost: address 50 + length (length limit of 15) + /0 (50 to 65)
    */
    void saveData(String deviceId, String ssid, String pass, String mqttHost, String mqttPass, String topic) {
        saveDeviceId(deviceId);
        saveSsid(ssid);
        savePassword(pass);
        saveMqttHost(mqttHost);
        if (mqttPass.length() > 0)
            saveMqttPass(mqttPass);
        saveTopic(topic);
    }

    String loadDeviceID()
    {
        return EEPROM.readString(START_POSITION_DEVICEID);
    }

    void saveDeviceId(String deviceId)
    {
        if (isStringEmpty(deviceId))
        {
            Serial.println("Exception, deviceId cannot be empty");
            return;
        }
        if (deviceId.length() > LENGTH_LIMIT_DEVICEID)
        {
            Serial.println("Exception, deviceId cannot be greater than 10");
            return;
        }
        EEPROM.writeString(START_POSITION_DEVICEID, deviceId);
        Serial.println("Device ID registered successfully");
    }

    String loadSsid()
    {
        return EEPROM.readString(START_POSITION_SSID);
    }

    void saveSsid(String ssid)
    {
        if (isStringEmpty(ssid))
        {
            Serial.println("Exception, ssid cannot be empty");
            return;
        }
        if (deviceId.length() > LENGTH_LIMIT_SSID)
        {
            Serial.println("Exception, ssid cannot be greater than 10");
            return;
        }
        EEPROM.writeString(START_POSITION_SSID, ssid);
        Serial.println("Ssid registered successfully");
    }
    
    bool isDeviceConfigured()
    {
        if (EEPROM.read(MAIN_POSITION_CONFIGURED) == 1)
            return true;
        return false;
    }

    bool isStringEmpty(String word) {
        return word.isEmpty();
    }
};

#endif