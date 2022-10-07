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

#define EEPROM_SIZE 128

// declarations
LocalStorage storage;
WebConnector webConnector(storage);
WifiClient wifiClient;
ConfigMqtt configMqtt(wifiClient, storage);
bool preLoopExecuted = false;
string deviceId = "esp32", ssid = "10.42.0.0:5901", pass = "lab@iiot";

void preLoop()
{
    if (!webConnector.isDeviceConfigured())
    {
        Serial.println("device not configured, configuring...");
        webConnector.configureServer(deviceId, ssid, pass);
    }
    else
    {
        string loadedData = webConnector.loadData(deviceId);
        int len = loadedData.length();
        char char_array[len + 1];
        strcpy(char_array, loadedData.c_str());
        Serial.println(char_array);
    }
    preLoopExecuted = true;
}

void setup()
{
    EEPROM.begin(EEPROM_SIZE);
    Serial.begin(115200);
    delay(3000);
}

void loop()
{
    if (!preLoopExecuted)
    {
        delay(5000);
        EEPROM.write(0, 0);
        delay(3000);
        preLoop();
    }
    
    Serial.print("value in flash memory: ");
    Serial.println(storage.loadDeviceID());
    delay(5000);
    storage.saveDeviceId("deviceId");
}
