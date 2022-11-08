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
        const char* topic;
        unsigned long waitTime;
        unsigned long reconnect;

    public:
        Mqtt(WifiConnector &wifiConnector) : wifiConnector(wifiConnector) {
            this->waitTime = 0;
            this->reconnect = 0;
        }

        void displayWifiNetworks() {
            //wifiConnector.displayWifiNetworks();
        }

        void loopMqtt() {
            MQTT.loop();
            if(!MQTT.connected()){
                if(reconnect< millis()){ //Executa a solicitacao de conexao a cada 5 segundos
                Serial.println("");
                Serial.println("Connecting to server...");
                //Solicita a conexao com o servidor utilizando os parametros "ID_DISPOSITIVO", "TOKEN_API" e "SENHA_ID" 
                if(!MQTT.connect(deviceId)){ 
                    Serial.println("Failed to connect to MQTT server");
                } else {
                    Serial.println("MQTT connected!");
                    Serial.print("Estabilishing subscription to topic: ");
                    Serial.println(topic);
                    if(!MQTT.subscribe(topic)){
                        Serial.println("Failed to subscribe to topic");
                        Serial.println("Disconnecting from MQTT server...");
                        MQTT.disconnect();
                        Serial.println("MQTT disconnected");
                    } else {
                        Serial.println("MQTT subscription done!");
                    }
                }
                reconnect = millis() + 5000; //Atualiza a contagem de tempo
                }
            }
        }

        void connectMQTT(const char* ssid, const char* password, const char* topic, const char* mqttIP, int socket) {
            wifiConnector.connectWiFiClient(ssid, password);
            // after WiFi is connected, connect MQTT
            Serial.print("MQTT server configured: ");
            Serial.print(mqttIP);
            Serial.print(" : ");
            Serial.println(socket);
            MQTT.setServer(mqttIP, socket);
            Serial.println("MQTT server set");
            MQTT.setCallback(mqttCallback);
            this->deviceId = deviceId;
            this->topic = topic;
            delay(1000);
        }
        /*
        void subscribe() {
            // keep connection active
            //MQTT.loop();
            if (!MQTT.connected()) {
                if (reconnect < millis()) {
                    Serial.println("");
                    Serial.println("Connecting to server...");
                    if (!MQTT.connect(deviceId, tokenApi, mqttPassword)) {
                        Serial.println("Failed to connect to MQTT server");
                        Serial.println("Disconnecting from server...");
                        MQTT.disconnect();
                        Serial.println("Disconnected");
                    }
                    else {
                        Serial.println("Subscription done!");
                    }
                }
                reconnect = millis() + 5000;
            }
        }

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
        static void mqttCallback(char* thing, byte* payload, unsigned int length) {
            Serial.println("entered callback");
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