#include <EEPROM.h>
#include <iostream>
#include <WiFi.h>
#include "localstorage.hpp"
#include "webconnector.hpp"
#include "wificlient.hpp"
#include "configmqtt.hpp"
#include <string>
#include <cstring>

using namespace std;

#define EEPROM_SIZE 1

// declarations
LocalStorage storage;
WebConnector webConnector(storage);
WifiClient wifiClient;
ConfigMqtt configMqtt(wifiClient, storage);
bool preLoopExecuted = false;

void preLoop(){
    Serial.println("Entered preloop");
    string deviceId = "arduino";
    if (!webConnector.isDeviceConfigured(deviceId)) {
        Serial.println("device not found, configuring...");
        webConnector.configureServer("arduino");
    } else {
        string loadedData = webConnector.loadData(deviceId);
        int len = loadedData.length();
        char char_array[len+1];
        strcpy(char_array, loadedData.c_str());
        Serial.println(char_array);
    }
    preLoopExecuted=true;
}


void setup() {
    EEPROM.begin(EEPROM_SIZE);
    Serial.begin(115200);
    delay(3000);
}

void loop() {
    if (!preLoopExecuted){
        Serial.println("print");
        preLoop();

        int value_stored = EEPROM.read(0);
        if (value_stored != 1) {
            EEPROM.write(0, 1);
            EEPROM.commit();
            Serial.println("Value 1 written in flash at memory postition 0");
        }
    }
    Serial.print("value in flash memory: ");
    Serial.println(EEPROM.read(0));
    delay(5000);
}
