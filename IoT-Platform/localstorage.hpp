#ifndef LOCALSTORAGE_HPP
#define LOCALSTORAGE_HPP

#include <iostream>
#include <Arduino.h>
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
    void saveData(String deviceId, String ssid, String password, String mqttHost, 
                  String mqttPass, String topic) {
        saveDeviceId(deviceId);
        saveSsid(ssid);
        savePassword(password);
        saveMqttHost(mqttHost);
        if (mqttPass.length() > 0)
            saveMqttPass(mqttPass);
        saveTopic(topic);
        EEPROM.write(0, 1);
        EEPROM.commit();
    }

    String loadData(bool mqttPassExists) {
        String dataReturn = "";
        if (mqttPassExists)
            dataReturn =+ "Device ID: " + loadInfo(START_POSITION_DEVICEID, LENGTH_LIMIT_DEVICEID) 
                          + ", SSID: " + loadInfo(START_POSITION_SSID, LENGTH_LIMIT_SSID) 
                          + ", Password: " + loadInfo(START_POSITION_PASSWORD, LENGTH_LIMIT_PASSWORD)  
                          + ", MQTT Host: " + loadInfo(START_POSITION_MQTTHOST, LENGTH_LIMIT_MQTTHOST)  
                          + ", MQTT Password: " + loadInfo(START_POSITION_MQTTPASS, LENGTH_LIMIT_MQTTPASS)  
                          + ", Topic: " + loadInfo(START_POSITION_TOPIC, LENGTH_LIMIT_TOPIC) ;
        else
            dataReturn =+ "Device ID: " + loadInfo(START_POSITION_DEVICEID, LENGTH_LIMIT_DEVICEID) 
                          + ", SSID: " + loadInfo(START_POSITION_SSID, LENGTH_LIMIT_SSID) 
                          + ", Password: " + loadInfo(START_POSITION_PASSWORD, LENGTH_LIMIT_PASSWORD) 
                          + ", MQTT Host: " + loadInfo(START_POSITION_MQTTHOST, LENGTH_LIMIT_MQTTHOST) 
                          + ", MQTT Password: [empty] , Topic: " 
                          + loadInfo(START_POSITION_TOPIC, LENGTH_LIMIT_TOPIC);
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

    char* loadTopic() {
        char outCharArray[LENGTH_LIMIT_TOPIC];
        EEPROM.readString(START_POSITION_TOPIC, outCharArray, LENGTH_LIMIT_TOPIC);
        return outCharArray;
    }

    const char* loadSsid() {
        char outCharArray[LENGTH_LIMIT_SSID];
        EEPROM.readString(START_POSITION_SSID, outCharArray, LENGTH_LIMIT_SSID);
        const char* outConstChar = outCharArray;
        return outConstChar;
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

    void saveDeviceId(String deviceId) {
        if (deviceId.isEmpty()) {
            Serial.println("Exception, deviceId cannot be empty");
            return;
        }
        if (deviceId.length() > LENGTH_LIMIT_DEVICEID) {
            lengthErrorMessage(deviceId, LENGTH_LIMIT_DEVICEID);
            return;
        }
        EEPROM.writeString(START_POSITION_DEVICEID, deviceId.c_str());
        successMessage(deviceId);
    }

    void saveSsid(String ssid) {
        if (ssid.isEmpty()) {
            Serial.println("Exception, ssid cannot be empty");
            return;
        }
        if (ssid.length() > LENGTH_LIMIT_SSID) {
            lengthErrorMessage(ssid, LENGTH_LIMIT_SSID);
            return;
        }
        EEPROM.writeString(START_POSITION_SSID, ssid.c_str());
        successMessage(ssid);
    }

    void savePassword(String password) {
        if (password.isEmpty()) {
            Serial.println("Exception, password cannot be empty");
            return;
        }
        if (password.length() > LENGTH_LIMIT_PASSWORD) {
            lengthErrorMessage(password, LENGTH_LIMIT_PASSWORD);
            return;
        }
        EEPROM.writeString(START_POSITION_PASSWORD, password.c_str());
        successMessage(password);
    }

    void saveMqttHost(String mqttHost) {
        if (mqttHost.isEmpty()) {
            Serial.println("Exception, mqttHost cannot be empty");
            return;
        }
        if (mqttHost.length() > LENGTH_LIMIT_MQTTHOST) {
            lengthErrorMessage(mqttHost, LENGTH_LIMIT_MQTTHOST);
            return;
        }
        EEPROM.writeString(START_POSITION_MQTTHOST, mqttHost.c_str());
        successMessage(mqttHost);
    }

    String loadMqttPass()
    {
        char outCharArray[LENGTH_LIMIT_MQTTPASS];
        EEPROM.readString(START_POSITION_MQTTPASS, outCharArray, LENGTH_LIMIT_MQTTPASS);
        String outString = "";
        for (int i = 0; i < strlen(outCharArray); i++)
        {
            outString = outString + outCharArray[i];
        }
        return outString;
    }

    void saveMqttPass(String mqttPass) {
        if (mqttPass.length() > LENGTH_LIMIT_MQTTPASS) {
            lengthErrorMessage(mqttPass, LENGTH_LIMIT_MQTTPASS);
            return;
        }
        EEPROM.writeString(START_POSITION_MQTTPASS, mqttPass.c_str());
        successMessage(mqttPass);
    }

    void saveTopic(String topic)
    {
        if (topic.isEmpty())
        {
            Serial.println("Exception, topic cannot be empty");
            return;
        }
        if (topic.length() > LENGTH_LIMIT_TOPIC)
        {
            lengthErrorMessage(topic, LENGTH_LIMIT_TOPIC);
            return;
        }
        EEPROM.writeString(START_POSITION_TOPIC, topic.c_str());
        successMessage(topic);
    }
    
    bool isDeviceConfigured() {
        if (EEPROM.read(MAIN_POSITION_CONFIGURED) == 1)
            return true;
        return false;
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