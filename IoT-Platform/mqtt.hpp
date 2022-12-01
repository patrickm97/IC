#ifndef MQTT_HPP
#define MQTT_HPP

#include "wificonnector.hpp"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <iostream>
using namespace std;

class Mqtt {
    public:
    typedef std::function<void(uint8_t*, unsigned int)> interpretConfigCallback;

    private:
        WifiConnector &wifiConnector;
        WiFiClient client;
        PubSubClient MQTT;
        char deviceId[50];
        char topic[50];
        char mqttIP[50];
        char ssid[50];
        char ssidPassword[50];
        uint16_t socket;
        unsigned long waitTime;
        unsigned long reconnect;
        
        interpretConfigCallback interpretCallback = nullptr;
        
    public:
        Mqtt(WifiConnector &wifiConnector) : wifiConnector(wifiConnector), MQTT(client) {
            this->waitTime = 0;
            this->reconnect = 0;
            this->socket = 0;
        }

        void setInterpretConfigCallback(const interpretConfigCallback& interpretCallback){
            this->interpretCallback = interpretCallback;
        }

        void subscribeInfo() {
            Serial.println("");
            Serial.printf("Mqtt Server IP: %s PORT %d\n", this->mqttIP, this->socket);
            Serial.println("Connecting to server...");
            MQTT.setServer(this->mqttIP, this->socket);
        }
        
        void subscribeAddSensor() {
            char addTopic[10] = "add";
            Serial.print("Estabilishing subscription to topic: ");
            Serial.println(addTopic);
            MQTT.subscribe(addTopic);
        }

        void publish(String sensor, String value) {
            DynamicJsonDocument json(JSON_OBJECT_SIZE(2));
            json[sensor.c_str()] = value;
            size_t message_size = measureJson(json) + 1;
            char message[message_size];
            serializeJson(json, message, message_size);
            Serial.print("Message sent: ");
            Serial.println(message);
            MQTT.publish(this->topic, message);
        }
        
        void loopMqtt() {
            MQTT.loop();
            if(!MQTT.connected()) {
                if(reconnect < millis()) {
                    subscribeInfo();
                if (!MQTT.connect(deviceId)) { 
                    Serial.println("Failed to connect to MQTT server");
                } else {
                    MQTT.publish("dev", "Hello!");
                    Serial.println("MQTT connected!");
                    Serial.print("Estabilishing subscription to topic: ");
                    Serial.println(topic);
                    subscribeAddSensor();
                    if (!MQTT.subscribe(topic)) {
                        Serial.println("Failed to subscribe to topic");
                        Serial.println("Disconnecting from MQTT server...");
                        MQTT.disconnect();
                        Serial.println("MQTT disconnected");
                    } else {
                        Serial.println("MQTT subscription done!");
                    }
                }
                reconnect = millis() + 5000;
                }
            }
        }

        void connectMQTT(const char* ssid, const char* password, const char* topic, const char* mqttIP, uint16_t socket, const char* deviceId) {
            strcpy(this->topic, topic);
            strcpy(this->deviceId, deviceId);
            strcpy(this->mqttIP, mqttIP);
            strcpy(this->ssid, ssid);
            strcpy(this->ssidPassword, password);
            this->socket = socket;
            wifiConnector.connectWiFiClient(this->ssid, this->ssidPassword);
            // after WiFi is connected, connect MQTT
            Serial.print("MQTT server configured: ");
            Serial.print(this->mqttIP);
            Serial.print(" : ");
            Serial.println(socket);
            MQTT.setServer(this->mqttIP, socket);
            Serial.println("MQTT server set");
            MQTT.setCallback([this](char* thing, uint8_t* msg, unsigned int s) {
                this->mqttCallback(thing, msg, s);
            });
            delay(1000);
        }
        
        void mqttCallback(char* topic, byte* payload, unsigned int length) {
            if (strcmp(topic,"add") == 0)
                this->interpretCallback(payload, length);
            
            // variable to store the message received in subscribe
            String message;
            
            // save the message characters in message
            for (int i = 0; i < length; i++) {
                char c = (char)payload[i];
                message += c;
            }

            Serial.print("Message received in subscribe: ");
            Serial.println(message);

            if (topic == "add") {
                int n = message.length();
                char json[n+1];
                strcpy(json, message.c_str());
                Serial.print("String to char[]: ");
                Serial.println(json);
            }
        }
};

#endif