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
bool preLoopExecuted = false;
//String deviceId = "esp32", ssid = "iotserver", password = "lab@iiot", mqttIP = "10.42.0.1", mqttPass = "pass", topic = "dev";
// int  mqttSocket = 5901;
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
//int a = 0;

void loop()
{
    /*
    if (a == 0) {
        delay(5000);
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
            delay(1000);
        }
        if (!mqttConnected) {
            String ssid = storage.loadSsid();
            String password = storage.loadPassword();
            String topic = storage.loadTopic();
            String mqttIP = storage.loadMqttIP();
            int socket = storage.loadMqttSocket();
            String mqttPass = storage.loadMqttPass();
            delay(1000);
            configMqtt.connectMqtt(ssid.c_str(), password.c_str(), topic.c_str(), mqttIP.c_str(), socket, mqttPass.c_str());
            mqttConnected = true;
        }

        if (mqttConnected) {
            configMqtt.loopMqtt();
            //configMqtt.subscribe();
        }
    }
}
