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
            this->tokenApi = "oi";
        }

        void displayWifiNetworks() {
            wifiConnector.displayWifiNetworks();
        }

        void connectMQTT(const char* ssid, const char* password, char* topic, char* mqttHost, int socket, char* mqttPass) {
            wifiConnector.connectWiFi(ssid, password);
            // after WiFi is connected, connect MQTT
            Serial.print("Server configured: ");
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
            cout << "subscribe";
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

        void callback() {
            cout << "callback";
        }

        
};

#endif