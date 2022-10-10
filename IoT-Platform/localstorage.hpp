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
    }

    String loadData(bool mqttPassExists) {
        String dataReturn;
        if (mqttPassExists)
            dataReturn = "Device ID: " + loadDeviceID() + ", SSID: " + loadSsid() + ", Password: " 
                            + loadPassword() + ", MQTT Host: " + loadMqttHost() + ", MQTT Password: " 
                            + loadMqttPass() + ", Topic: " + loadTopic();
        else
            dataReturn = "Device ID: " + loadDeviceID() + ", SSID: " + loadSsid() + ", Password: " 
                            + loadPassword() + ", MQTT Host: " + loadMqttHost() + ", MQTT Password: " 
                            + "[empty] , Topic: " + loadTopic();
        return dataReturn;
    }

    String loadDeviceID() {
        return EEPROM.readString(START_POSITION_DEVICEID);
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
        EEPROM.writeString(START_POSITION_DEVICEID, deviceId);
        successMessage(deviceId);
    }

    String loadSsid() {
        return EEPROM.readString(START_POSITION_SSID);
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
        EEPROM.writeString(START_POSITION_SSID, ssid);
        successMessage(ssid);
    }

    String loadPassword() {
        return EEPROM.readString(START_POSITION_PASSWORD);
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
        EEPROM.writeString(START_POSITION_PASSWORD, password);
        successMessage(password);
    }

    String loadMqttHost() {
        return EEPROM.readString(START_POSITION_MQTTHOST);
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
        EEPROM.writeString(START_POSITION_MQTTHOST, mqttHost);
        successMessage(mqttHost);
    }

    String loadMqttPass() {
        return EEPROM.readString(START_POSITION_MQTTPASS);
    }

    void saveMqttPass(String mqttPass) {
        if (mqttPass.length() > LENGTH_LIMIT_MQTTPASS) {
            lengthErrorMessage(mqttPass, LENGTH_LIMIT_MQTTPASS);
            return;
        }
        EEPROM.writeString(START_POSITION_MQTTPASS, mqttPass);
        successMessage(mqttPass);
    }

    String loadTopic()
    {
        return EEPROM.readString(START_POSITION_MQTTHOST);
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
        EEPROM.writeString(START_POSITION_TOPIC, topic);
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