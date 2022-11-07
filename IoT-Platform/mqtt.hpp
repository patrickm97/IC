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

    public:
        Mqtt(WifiConnector &wifiConnector) : wifiConnector(wifiConnector) {
            this->tokenApi = "token";
        }

        void displayWifiNetworks() {
            //wifiConnector.displayWifiNetworks();
        }

        void loopMqtt() {
            MQTT.loop();
            if(!MQTT.connected()){ //Se nao estiver conectado
                if(reconnect< millis()){ //Executa a solicitacao de conexao a cada 5 segundos
                Serial.println("");
                Serial.println("Conectando ao servidor...");
                //Solicita a conexao com o servidor utilizando os parametros "ID_DISPOSITIVO", "TOKEN_API" e "SENHA_ID" 
                if(!MQTT.connect(deviceId, tokenApi, mqttPassword)){ 
                    Serial.println("Falha na conexao com o servidor.");
                } else {
                    Serial.println("Conectado!");
                    Serial.print("Estabelecendo inscricao com ");
                    Serial.println(topic);
                    //Solicita a inscricao no "TOPICO"
                    if(!MQTT.subscribe(topic)){
                    Serial.println("Erro ao se inscrever no topico");
                    Serial.println("Desconectando do servidor...");
                    MQTT.disconnect(); //Desconecta a placa do servidor
                    Serial.println("Desconectado.");
                    } else {
                    Serial.println("Inscricao concluida!");
                    }
                }
                reconnect = millis() + 5000; //Atualiza a contagem de tempo
                }
            }
        }

        void connectMQTT(const char* ssid, const char* password, const char* topic, const char* mqttHost, int socket, const char* mqttPass) {
            wifiConnector.connectWiFiClient(ssid, password);
            // after WiFi is connected, connect MQTT
            Serial.print("MQTT server configured: ");
            Serial.print(mqttHost);
            Serial.print(" : ");
            Serial.println(socket);
            MQTT.setServer(mqttHost, socket);
            Serial.println("MQTT server set");
            MQTT.setCallback(mqttCallback);
            this->deviceId = deviceId;
            this->mqttPassword = mqttPass;
            this->topic = topic;
            delay(1000);
            //Serial.println("antes do publish");
            //MQTT.publish("conexao", this->deviceId);
            //Serial.println("depois do publish");
        }

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