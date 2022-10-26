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
        const char* deviceId;
        const char* mqttPassword;
        const char* tokenApi;
        const char* topic;
        unsigned long waitTime;
        unsigned long reconnect;

        void loopMQTT() {
            // keep the connection active
            MQTT.loop();
        }

    public:
        Mqtt(WifiConnector &wifiConnector) : wifiConnector(wifiConnector) {
            this->tokenApi = "token";
        }

        void displayWifiNetworks() {
            //wifiConnector.displayWifiNetworks();
        }

        void connectMQTT(const char* ssid, const char* password, char* topic, char* mqttHost, int socket, char* mqttPass) {
            //wifiConnector.connectWiFi(ssid, password);
            // after WiFi is connected, connect MQTT
            Serial.print("MQTT server configured: ");
            Serial.print(mqttHost);
            Serial.print(" : ");
            Serial.println(socket);
            MQTT.setServer(mqttHost, socket);
            Serial.println("MQTT server set");
            delay(500);
            this->deviceId = deviceId;
            this->mqttPassword = mqttPass;
            this->topic = topic;
            delay(500);
        }

        void subscribe() {
            MQTT.setCallback(mqttCallback);
        }

        void publish() {
            loopMQTT();
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

        static void mqttCallback(char* thing, byte* payload, unsigned int length) {
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