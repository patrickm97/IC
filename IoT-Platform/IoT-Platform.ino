#include <EEPROM.h>
#include <iostream>
#include <WiFi.h>
#include "localstorage.hpp"
#include "webconnector.hpp"
#include "wificlient.hpp"
#include "configmqtt.hpp"

using namespace std;

#define EEPROM_SIZE 512

// declarations
LocalStorage storage;
WebConnector webConnector(storage);
WifiClient wifiClient;
ConfigMqtt configMqtt(wifiClient, storage);
bool preLoopExecuted = false, mqttPassExists = false;;
String deviceId = "esp32", ssid = "10.42.0.0:5901", password = "lab@iiot", 
       mqttHost = "190.186.5.1:8000", mqttPass = "pass", topic = "dev-iot";

void printInfo(bool mqttPassExists) 
{
    webConnector.loadData(mqttPassExists);
}

void preLoop() {
    if (!webConnector.isDeviceConfigured()) 
    {
        Serial.println("Device not configured, configuring...");
        webConnector.saveData(deviceId, ssid, password, mqttHost, mqttPass, topic);
    }
    else
    {
        Serial.println("Device already registered, printing info...");
        delay(2000);
    }
    if (!storage.loadMqttPass().isEmpty())
        mqttPassExists = true;
    preLoopExecuted = true;
}

void setup() 
{
    EEPROM.begin(EEPROM_SIZE);
    Serial.begin(115200);
    delay(3000);
}

int v = 0;
void testaGravaString(){
    v++;
    String s = "blablabla";
    s.concat(v);
    EEPROM.writeString(1, s.c_str());
    Serial.printf("%s %s\n", "Gravado: ", s);
}

void testaLeiaString(){
    char teste[100];
    EEPROM.readString(1, teste, 100);
    Serial.printf("%s %s\n", "Memoria", teste);

}


void loopTeste(){
    String s = Serial.readString();
    Serial.printf("%s%s%s\n","->",s,"<-");
    if (s.compareTo("w\n") == 0){
        testaGravaString();
    }
    if (s.compareTo("r\n") == 0){
        testaLeiaString();
    }
}


void loop() {
    if (!preLoopExecuted)
    {
        delay(2000);
        //EEPROM.write(0, 0);
        delay(1000);
        preLoop();
    }
    
    Serial.println("\n-------------------------Info in flash memory-------------------------");
    Serial.println(webConnector.loadData(mqttPassExists));
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
