#include <iostream>
#include <WiFi.h>
#include "localstorage.hpp"
#include "webconnector.hpp"
#include "wificlient.hpp"
#include "configmqtt.hpp"
#include <string>
#include <cstring>

using namespace std;

/*
* no ardino.json
* board: "esp32:esp32:esp32"
*/

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
    Serial.begin(115200);
    delay(3000);
}

void loop() {
    if (!preLoopExecuted){
        Serial.println("print");
        preLoop();
    }
    Serial.println("loop executed");
    delay(5000);
}
