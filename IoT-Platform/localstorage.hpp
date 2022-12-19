#ifndef LOCALSTORAGE_HPP
#define LOCALSTORAGE_HPP

#include <iostream>
#include <Arduino.h>
#include <cstring>
#include <EEPROM.h>
#include "configParams.hpp"
using namespace std;

#define MAIN_POSITION_CONFIGURED 0

#define START_POSITION_DEVICEID 1
#define START_POSITION_SSID 13
#define START_POSITION_PASSWORD 35
#define START_POSITION_MQTTIP 51
#define START_POSITION_MQTTSOCKET 67
#define START_POSITION_TOPIC 72

#define LENGTH_LIMIT_DEVICEID 10
#define LENGTH_LIMIT_SSID 20
#define LENGTH_LIMIT_PASSWORD 15
#define LENGTH_LIMIT_MQTTIP 15
#define LENGTH_LIMIT_MQTTSOCKET 4
#define LENGTH_LIMIT_TOPIC 10

class LocalStorage
{
public:
    /* reserved addresses in flash memory:
    *  deviceId: address 1 + 10 + /0 (1 to 12)
    *  ssid: address 13 + 20 + /0 (13 to 34)
    *  password: address 35 + 15 + /0 (35 to 50)
    *  mqttIP: address 51 + 15 + /0 (51 to 66)
    *  mqttSocket: address 67 + 4 + /0 (67 to 71)
    *  topic: address 72 + 10 + /0 (72 to 83)
    */

    void saveConfig(ConfigParams config){
        saveInfo(config.deviceId, START_POSITION_DEVICEID, LENGTH_LIMIT_DEVICEID);
        saveInfo(config.ssid, START_POSITION_SSID, LENGTH_LIMIT_SSID);
        saveInfo(config.password, START_POSITION_PASSWORD, LENGTH_LIMIT_PASSWORD);
        saveInfo(config.mqttIP, START_POSITION_MQTTIP, LENGTH_LIMIT_MQTTIP);
        saveInfo(config.mqttSocket, START_POSITION_MQTTSOCKET, LENGTH_LIMIT_MQTTSOCKET);
        saveInfo(config.topic, START_POSITION_TOPIC, LENGTH_LIMIT_TOPIC);
        EEPROM.write(0, 1); // value 1 in position 0
        delay(50);
        EEPROM.commit();
    }

    String loadData() {
        String dataReturn = "";
        dataReturn += "Device ID: " + loadInfo(START_POSITION_DEVICEID, LENGTH_LIMIT_DEVICEID) 
                        + ", SSID: " + loadInfo(START_POSITION_SSID, LENGTH_LIMIT_SSID) 
                        + ", Password: " + loadInfo(START_POSITION_PASSWORD, LENGTH_LIMIT_PASSWORD)  
                        + ", MQTT Host: " + loadInfo(START_POSITION_MQTTIP, LENGTH_LIMIT_MQTTIP)
                        + ", MQTT Socket: " + loadInfo(START_POSITION_MQTTSOCKET, LENGTH_LIMIT_MQTTSOCKET) 
                        + ", Topic: " + loadInfo(START_POSITION_TOPIC, LENGTH_LIMIT_TOPIC);
        return dataReturn;
    }

    String loadInfo(int startPos, int lengthLimit)
    {
        char outCharArray[lengthLimit];
        EEPROM.readString(startPos, outCharArray, lengthLimit);
        String outString = "";
        for (int i = 0; i < strlen(outCharArray); i++)
            outString += outCharArray[i];
        return outString;
    }

    String loadDeviceId() {
        return loadInfo(START_POSITION_DEVICEID, LENGTH_LIMIT_DEVICEID);
    }

    String loadTopic() {
        return loadInfo(START_POSITION_TOPIC, LENGTH_LIMIT_TOPIC);
    }

    String loadSsid() {
        return loadInfo(START_POSITION_SSID, LENGTH_LIMIT_SSID);
    }

    String loadPassword() {
        return loadInfo(START_POSITION_PASSWORD, LENGTH_LIMIT_PASSWORD);
    }

    String loadMqttIP() {
        return loadInfo(START_POSITION_MQTTIP, LENGTH_LIMIT_MQTTIP);
    }

    uint16_t loadMqttSocket() {
        return loadInfo(START_POSITION_MQTTSOCKET, LENGTH_LIMIT_MQTTSOCKET).toInt();
    }
    
    bool isDeviceConfigured() {
        if (EEPROM.read(MAIN_POSITION_CONFIGURED) == 1)
            return true;
        return false;
    }

    void saveInfo(String info, int startPos, int lengthLimit) {
        if (info.isEmpty()) {
            Serial.println("Exception, info cannot be empty");
            return;
        }
        if (info.length() > lengthLimit) {
            lengthErrorMessage(info, lengthLimit);
            return;
        }
        EEPROM.writeString(startPos, info.c_str());
        successMessage(info);
    }

    void successMessage(String word) {
        Serial.print(word);
        Serial.println(" registered successfully");
    }

    void lengthErrorMessage(String word, int length) {
        Serial.print("Exception ");
        Serial.print(word);
        Serial.print(" cannot be greater than ");
        Serial.print(length);
    }
};

#endif