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
    public:
        Mqtt(WifiConnector &wifiConnector) : wifiConnector(wifiConnector) {
        }

        void displayWifiNetworks() {
            wifiConnector.displayWifiNetworks();
        }

        void connectMQTT(const char* ssid, const char* password, char* topic, const uint16_t socket) {
            wifiConnector.connectWiFi(ssid, password);
            // after WiFi is connected, connect MQTT
            Serial.print("Wifi connection configured: ");
            const char* server = ssid;
            Serial.print(server);
            Serial.print(" : ");
            Serial.println()
        }

        void subscribe() {
            cout << "subscribe";
        }

        void publish() {
            cout << "publish";
        }

        void callback() {
            cout << "callback";
        }
};

#endif