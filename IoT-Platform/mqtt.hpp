#ifndef MQTT_HPP
#define MQTT_HPP

#include "wificonnector.hpp"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <iostream>
using namespace std;

class Mqtt {
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
        float humidity;
        float temperature;
        char ALIAS1[50];
        char ALIAS2[50];
        long interval;
        
    public:
        Mqtt(WifiConnector &wifiConnector) : wifiConnector(wifiConnector), client(), MQTT(client) {
            this->waitTime = 0;
            this->reconnect = 0;
            this->humidity = 15.5;
            this->temperature = 21.3;
            this->socket = 0;
            strcpy(ALIAS1, "Humidity");
            strcpy(ALIAS2, "Temperature");
            this->interval = 10000;
        }

        void subscribe() {
            Serial.println("");
            Serial.printf("Mqtt Server IP: %s PORT %d\n", this->mqttIP, this->socket);
            Serial.println("Connecting to server...");
            MQTT.setServer(this->mqttIP, this->socket);
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
            
            // if (waitTime < millis()) {
            //     if (isnan(humidity) || isnan(temperature))
            //         Serial.println("Failed to read sensor data!");
            //     else {
            //         DynamicJsonDocument json(JSON_OBJECT_SIZE(2));
            //         json[ALIAS1] = temperature;
            //         json[ALIAS2] = humidity;
            //         size_t message_size = measureJson(json) + 1;
            //         char message[message_size];
            //         serializeJson(json, message, message_size);
            //         Serial.print("Message sent: ");
            //         Serial.println(message);
            //         MQTT.publish(this->topic, message);
            //     }
            //     waitTime = millis() + interval;
            // }
        }
        
        void loopMqtt() {
            MQTT.loop();
            if(!MQTT.connected()) {
                if(reconnect < millis()) {
                    subscribe(); 
                if (!MQTT.connect(deviceId)) { 
                    Serial.println("Failed to connect to MQTT server");
                } else {
                    MQTT.publish("dev", "Hello!");
                    Serial.println("MQTT connected!");
                    Serial.print("Estabilishing subscription to topic: ");
                    Serial.println(topic);
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
            } else {
                //publish();
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
            MQTT.setCallback([this](char* thing, uint8_t* msg, unsigned int s){
                this->mqttCallback(thing, msg, s);
            });
            delay(1000);
        }
        /*

        void publish() {
            // keep connection active
            //MQTT.loop();
            if (!MQTT.connected()) {
                if(reconnect < millis()) {
                    Serial.println("");
                    Serial.println("Reconnecting to server...");
                    if (!MQTT.connect(deviceId, tokenApi, mqttPassword)) {
                        Serial.println("MQTT server connection error");
                    } else {
                        Serial.println("MQTT connected!");
                        Serial.print("Publishing to: ");
                        Serial.print(topic);
                    }
                    reconnect = millis() + 5000;
                }
            } else {
                if (waitTime < millis()) {
                    // TODO - publish sensor info to topic
                }
            }
        }
        */
        void mqttCallback(char* thing, byte* payload, unsigned int length) {
            // variable to store the message received in subscribe
            String message;
            // save the message characters in message
            for (int i = 0; i < length; i++) {
                char c = (char)payload[i];
                message += c;
            }
            // TODO - handle message received
            Serial.print("Message received in subscribe: ");
            Serial.println(message);
        }
};

#endif