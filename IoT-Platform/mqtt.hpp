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
        typedef std::function<void(uint8_t*, unsigned int)> interpretOutCallback;

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
        interpretOutCallback interpretCallbackOut = nullptr;
        
    public:
        Mqtt(WifiConnector &wifiConnector) : wifiConnector(wifiConnector), MQTT(client) {
            this->waitTime = 0;
            this->reconnect = 0;
            this->socket = 0;
        }

        void setInterpretConfigCallback(const interpretConfigCallback& interpretCallback){
            this->interpretCallback = interpretCallback;
        }

        void setInterpretOutCallback(const interpretOutCallback& interpretCallbackOut){
            this->interpretCallbackOut = interpretCallbackOut;
        }

        void subscribeConnect() {
            Serial.println("");
            Serial.printf("Mqtt Server IP: %s PORT %d\n", this->mqttIP, this->socket);
            Serial.println("Connecting to server...");
            MQTT.setServer(this->mqttIP, this->socket);
        }
        
        void subscribeAddSensor() {
            char addTopic[4] = "add";
            Serial.print("Estabilishing subscription to topic: ");
            Serial.println(addTopic);
            MQTT.subscribe(addTopic);
            Serial.print("MQTT subscription to topic ");
            Serial.print(addTopic);
            Serial.println(" done!");
        }

        void subscribeTopic() {
            char outTopic[4] = "out";
            Serial.print("Estabilishing subscription to topic: ");
            Serial.println(outTopic);
            MQTT.subscribe(outTopic);
            Serial.print("MQTT subscription to topic ");
            Serial.print(outTopic);
            Serial.println(" done!");
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
                    subscribeConnect();
                if (!MQTT.connect(deviceId)) {
                    Serial.println("Failed to connect to MQTT server");
                } else {
                    MQTT.publish("dev", "Hello!");
                    Serial.println("MQTT connected!");
                    Serial.print("Estabilishing subscription to topic: ");
                    Serial.println(topic);
                    subscribeAddSensor();
                    subscribeTopic();
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
            // after WiFi Client is connected, connect MQTT
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
            if (strcmp(topic, "add") == 0)
                this->interpretCallback(payload, length);

            if (strcmp(topic, "out") == 0)
                this->interpretCallbackOut(payload, length);
                
            // variable to store the message received in subscribe
            String message;
            
            // save the message characters in message
            for (int i = 0; i < length; i++) {
                char c = (char)payload[i];
                message += c;
            }

            Serial.print("Message received in subscribe: ");
            Serial.println(message);
        }
};

#endif