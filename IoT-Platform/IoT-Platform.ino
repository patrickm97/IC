#include <EEPROM.h>
#include <iostream>
#include <WiFi.h>
#include "localstorage.hpp"
#include "webconnector.hpp"
#include "wificlient.hpp"
#include "configmqtt.hpp"

using namespace std;

#define EEPROM_SIZE 128

// declarations
LocalStorage storage;
WebConnector webConnector(storage);
WifiClient wifiClient;
ConfigMqtt configMqtt(wifiClient, storage);
bool preLoopExecuted = false, mqttPassExists = false;;
String deviceId = "esp32", ssid = "10.42.0.0:5901", password = "lab@iiot", 
       mqttHost = "190.186.5.1:8000", mqttPass = "", topic = "dev-iot";

void printInfo(bool mqttPassExists) {
    webConnector.loadData(mqttPassExists);
}

void preLoop() {
    if (!webConnector.isDeviceConfigured()) {
        Serial.println("Device not configured, configuring...");
        webConnector.saveData(deviceId, ssid, password, mqttHost, mqttPass, topic);
    }
    else
        Serial.println("Device already registered, printing info...");
    if (!storage.loadMqttPass().isEmpty())
        mqttPassExists = true;
    preLoopExecuted = true;
}

void setup() {
    EEPROM.begin(EEPROM_SIZE);
    Serial.begin(115200);
    delay(3000);
}

void loop() {
    if (!preLoopExecuted) {
        delay(5000);
        //EEPROM.write(0, 0);
        delay(3000);
        preLoop();
    }
    
    Serial.println("--------------------Info in flash memory--------------------");
    Serial.println(webConnector.loadData(mqttPassExists));
    delay(5000);
}
