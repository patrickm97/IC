#include <EEPROM.h>
#include <iostream>
#include <WiFi.h>
#include "localstorage.hpp"
#include "webconnector.hpp"
#include "wificonnector.hpp"
#include "configmqtt.hpp"

using namespace std;

#define EEPROM_SIZE 512

// declarations
LocalStorage storage;
WifiConnector wifiConnector;
WebConnector webConnector(wifiConnector, storage);
ConfigMqtt configMqtt(wifiConnector, storage);
bool preLoopExecuted = false, mqttPassExists = false, wifiConnected = false;
String deviceId = "esp32", ssid = "10.42.0.0:5901", password = "lab@iiot", 
       mqttHost = "190.186.5.1:8000", mqttPass = "pass", topic = "dev-iot";
const uint16_t socket = 0;

void preLoop() {
    if (webConnector.isDeviceConfigured()) 
    {
        Serial.println("Device not configured, configuring...");
        webConnector.setupWebServer();
        //webConnector.saveData(deviceId, ssid, password, mqttHost, mqttPass, topic);
    }
    else
    {
        Serial.println("Device already registered, printing info...");
        delay(1000);
    }
    if (!storage.loadMqttPass(true).isEmpty())
        mqttPassExists = true;
    preLoopExecuted = true;
}

void setup() 
{
    EEPROM.begin(EEPROM_SIZE);
    Serial.begin(115200);
    delay(1000);
}

int v = 0;
void testaGravaString()
{
    v++;
    String s = "blablabla";
    s.concat(v);
    EEPROM.writeString(1, s.c_str());
    Serial.printf("%s %s\n", "Gravado: ", s);
    EEPROM.commit();
}

void testaLeiaString()
{
    char teste[100];
    EEPROM.readString(1, teste, 100);
    Serial.printf("%s %s\n", "Memoria", teste);
}

void loop2()
{
    String s = Serial.readString();
    Serial.printf("%s%s%s\n","->",s,"<-");
    if (s.compareTo("w\n") == 0){
        testaGravaString();
    }
    if (s.compareTo("r\n") == 0){
        testaLeiaString();
    }
}

void loop()
{
    if (!preLoopExecuted)
    {
        delay(2000);
        //EEPROM.write(0, 0);flash has 512 positions (0 to 511)
        delay(1000);
        preLoop();
        delay(1000);
        Serial.println("\n-------------------------Info in flash memory-------------------------");
        Serial.println(webConnector.loadData(mqttPassExists));
        Serial.println("");
        delay(1000);
    }
    
    if (!wifiConnected)
    {
        configMqtt.connectMqtt(storage.loadSsid(), storage.loadPassword(), storage.loadTopic(), storage.loadMqttHost(), storage.loadSocket(), storage.loadMqttPass());
        wifiConnected = true;
    }
    
    /*
    for (int i = 0; i < 512; i++)
    {
        Serial.print(EEPROM.readString(i));
        Serial.print(" ");
        if (i > 0 && i % 30 == 0)
        {
            Serial.println("");
        }  
    }
    */
    delay(10000);
}
