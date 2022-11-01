#ifndef LOCALSTORAGE_HPP
#define LOCALSTORAGE_HPP

#include <iostream>
#include <Arduino.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iterator>
#include <EEPROM.h>
#include "configParams.hpp"
using namespace std;

#define MAIN_POSITION_CONFIGURED 0
// 255.255.255.255:1000 -> len = 20
#define START_POSITION_DEVICEID 1
#define START_POSITION_SSID 13
#define START_POSITION_PASSWORD 35
#define START_POSITION_MQTTHOST 51
#define START_POSITION_MQTTPASS 72
#define START_POSITION_TOPIC 88

#define LENGTH_LIMIT_DEVICEID 10
#define LENGTH_LIMIT_SSID 20
#define LENGTH_LIMIT_PASSWORD 15
#define LENGTH_LIMIT_MQTTHOST 20
#define LENGTH_LIMIT_MQTTPASS 15
#define LENGTH_LIMIT_TOPIC 10

class LocalStorage
{
private:
public:
    /* reserved addresses in flash memory:
    *  deviceId: address 1 + length + /0 (1 to 12)
    *  ssid: address 13 + length + /0 (13 to 49)
    *  password: address 50 + length + /0 (50 to 65)
    *  mqttHost: address 66 + length + /0 (66 to 87)
    *  mqttPass: address 88 + length + /0 (88 to 104)
    *  topic: address 105 + length + /0 (105 to 116)
    */
   /*
    void saveData(String deviceId, String ssid, String password, String mqttHost, 
                  String mqttPass, String topic) {
        saveInfo(deviceId, START_POSITION_DEVICEID, LENGTH_LIMIT_DEVICEID);
        saveInfo(ssid, START_POSITION_SSID, LENGTH_LIMIT_SSID);
        saveInfo(password, START_POSITION_PASSWORD, LENGTH_LIMIT_PASSWORD);
        saveInfo(mqttHost, START_POSITION_MQTTHOST, LENGTH_LIMIT_MQTTHOST);
        if (mqttPass.length() > 0)
            saveInfo(mqttPass, START_POSITION_MQTTPASS, LENGTH_LIMIT_MQTTPASS);
        saveInfo(topic, START_POSITION_TOPIC, LENGTH_LIMIT_TOPIC);
        EEPROM.write(0, 1);
        delay(50);
        EEPROM.commit();
    }*/

    void saveConfig(ConfigParams config){
        saveInfo(config.deviceId, START_POSITION_DEVICEID, LENGTH_LIMIT_DEVICEID);
        saveInfo(config.ssid, START_POSITION_SSID, LENGTH_LIMIT_SSID);
        saveInfo(config.password, START_POSITION_PASSWORD, LENGTH_LIMIT_PASSWORD);
        saveInfo(config.mqttHost, START_POSITION_MQTTHOST, LENGTH_LIMIT_MQTTHOST);
        if (config.mqttPass.length() > 0)
            saveInfo(config.mqttPass, START_POSITION_MQTTPASS, LENGTH_LIMIT_MQTTPASS);
        saveInfo(config.topic, START_POSITION_TOPIC, LENGTH_LIMIT_TOPIC);
        EEPROM.write(0, 1); // value 1 in position 0
        delay(50);
        EEPROM.commit();
    }

    String loadData() {
        String dataReturn = "";
        dataReturn =+ "Device ID: " + loadInfo(START_POSITION_DEVICEID, LENGTH_LIMIT_DEVICEID) 
                        + ", SSID: " + loadInfo(START_POSITION_SSID, LENGTH_LIMIT_SSID) 
                        + ", Password: " + loadInfo(START_POSITION_PASSWORD, LENGTH_LIMIT_PASSWORD)  
                        + ", MQTT Host: " + loadInfo(START_POSITION_MQTTHOST, LENGTH_LIMIT_MQTTHOST)  
                        + ", MQTT Password: " + loadInfo(START_POSITION_MQTTPASS, LENGTH_LIMIT_MQTTPASS)  
                        + ", Topic: " + loadInfo(START_POSITION_TOPIC, LENGTH_LIMIT_TOPIC);
        return dataReturn;
    }

    String loadInfo(int startPos, int lengthLimit)
    {
        char outCharArray[lengthLimit];
        EEPROM.readString(startPos, outCharArray, lengthLimit);
        String outString = "";
        for (int i = 0; i < strlen(outCharArray); i++)
        {
            outString = outString + outCharArray[i];
        }
        return outString;
    }

    const char* loadTopic() {
        char outCharArray[LENGTH_LIMIT_TOPIC];
        EEPROM.readString(START_POSITION_TOPIC, outCharArray, LENGTH_LIMIT_TOPIC);
        const char* outConstChar = outCharArray;
        return outConstChar;
    }

    const char* loadSsid() {
        char outCharArray[LENGTH_LIMIT_SSID];
        EEPROM.readString(START_POSITION_SSID, outCharArray, LENGTH_LIMIT_SSID);
        const char* outConstChar = outCharArray;
        Serial.print("ssid in storage: ");
        Serial.println(outConstChar);
        return outCharArray;
    }

    const char* loadPassword() {
        char outCharArray[LENGTH_LIMIT_PASSWORD];
        EEPROM.readString(START_POSITION_PASSWORD, outCharArray, LENGTH_LIMIT_PASSWORD);
        const char* outConstChar = outCharArray;
        return outConstChar;
    }

    const char* loadMqttHost() {
        char outCharArray[LENGTH_LIMIT_MQTTHOST];
        EEPROM.readString(START_POSITION_MQTTHOST, outCharArray, LENGTH_LIMIT_MQTTHOST);
        const char* outConstChar = outCharArray;
        return outConstChar;
    }
    
    int loadSocket() {
        const char* mqttHost = loadMqttHost();
        String strMqttHost = mqttHost;
        int mqttHostLen = strMqttHost.length();
        int socketIdx = mqttHostLen - 5;
        String subMqttHost = strMqttHost.substring(socketIdx);
        char charSocket[4];
        strcpy(charSocket, subMqttHost.c_str());
        int socket = atoi(charSocket);
        return socket;
    }

    const char* loadMqttPass()
    {
        char outCharArray[LENGTH_LIMIT_MQTTPASS];
        EEPROM.readString(START_POSITION_MQTTPASS, outCharArray, LENGTH_LIMIT_MQTTPASS);
        const char* outConstChar = outCharArray;
        return outConstChar;
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