#include <EEPROM.h>
#include <iostream>
#include <WiFi.h>
#include "localstorage.hpp"
#include "wificonnector.hpp"
#include "configmqtt.hpp"

using namespace std;

// flash has 512 positions (0 to 511)
#define EEPROM_SIZE 512

// declarations
LocalStorage storage;
WifiConnector wifiConnector(storage);
ConfigMqtt configMqtt(wifiConnector, storage);
bool preLoopExecuted = false, wifiConnected = false, initialMessage = true;
//String deviceId = "esp32", ssid = "10.42.0.0:5901", password = "lab@iiot", mqttHost = "190.186.5.1:8000", mqttPass = "pass", topic = "dev-iot";
const uint16_t socket = 0;

void preLoop() {
    if (!wifiConnector.isDeviceConfigured()) {
        Serial.println("Device not configured, configuring...");
        wifiConnector.connectWifiServer();
        //wifiConnector.setupWebServer();
    }
    else {
        //Serial.println("Device already registered, connecting to WiFi client...");
        Serial.println("Device already registered, printing info...");
        wifiConnected = true;
        delay(1000);
    }
    preLoopExecuted = true;
}

void setZero() {
    EEPROM.write(0, 0);
    EEPROM.commit();
    delay(1000);
    Serial.println("0 set to 0");
}

void setup() 
{
    EEPROM.begin(EEPROM_SIZE);
    Serial.begin(115200);
    delay(1000);
    preLoop();
}

bool displayMessage = true, mqttConnected = false, subscribeMqtt = true;

void loop()
{
    /*
    if (a == 0) {
        setZero();
        a++;
    }*/
    if (!wifiConnector.isDeviceConfigured()) {
        wifiConnector.handleWebServerClient();
        delay(2);
    }
    else {
        if (displayMessage) {
            Serial.println("\n-------------------------Info in flash memory-------------------------");
            Serial.println(storage.loadData());
            Serial.println("");
            displayMessage = false;
        }
        if (!mqttConnected) {
            const char* ssid = storage.loadSsid();
            configMqtt.connectMqtt(ssid, storage.loadPassword(), storage.loadTopic(), 
            storage.loadMqttHost(), storage.loadSocket(), storage.loadMqttPass());
            mqttConnected = false;
        }
        if (mqttConnected && subscribeMqtt) {
            configMqtt.subscribe();
            subscribeMqtt = false;
        }
    }
    
}
