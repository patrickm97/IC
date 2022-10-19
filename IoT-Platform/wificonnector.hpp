#ifndef WIFICONNECTOR_HPP
#define WIFICONNECTOR_HPP

#include <iostream>
#include <string>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

using namespace std;

class WifiConnector {
    private:
        WiFiClient client;
        WebServer webServer;
        bool connectedToWifi;

    public:
        // constructor
        WifiConnector() {
            this->connectedToWifi = false;
        }
        // local Wi-Fi methods
        void displayWifiNetworks() {
            // get device MAC Address
            Serial.print("MAC address: ");
            Serial.println(WiFi.macAddress());

            // get local device IP address
            Serial.print("Device IP address: ");
            Serial.println(WiFi.localIP());

            // configure device as STA
            Serial.println("Configuring to 'Station' mode...");
            WiFi.mode(WIFI_STA);
            WiFi.disconnect();
            delay(1000);
            Serial.println("Configuration done\n");

            // scan wifi networks
            Serial.println("Scanning Wi-Fi networks..,");
            int number_of_networks = WiFi.scanNetworks();
            Serial.println("Scan complete");
            delay(1000);

            if (number_of_networks == 0) {
                Serial.println("No Wi-Fi network found");
            } 
            else {
                Serial.print(number_of_networks);
                Serial.println(" networks found");
                delay(500);
                for (int i = 0; i < number_of_networks; ++i) {
                    Serial.print(i + 1);
                    Serial.print(":");
                    Serial.print(WiFi.SSID(i));
                    Serial.print(" (");
                    Serial.print(WiFi.RSSI(i)); // signal quality
                    Serial.println(")");
                    delay(1000);
                }
            }
            Serial.println("");
        }

        void connectWiFi(const char* ssid, const char* password) {
            Serial.print("Connecting to network: ");
            Serial.println(ssid);
            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid, password);
            
            // wait for device to connect to network
            while (WiFi.status() != WL_CONNECTED) {
                delay(100);
                Serial.print(".");
            }

            // when connected, display IP address
            Serial.println("Connected!");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());
            this->connectedToWifi = true;
            setupWebServer();
            delay(1000);
        }

        WiFiClient getWifiClient() {
            return this->client;
        }

        // WebServer methods
        void setupWebServer() {
            if (MDNS.begin("esp32")) {
                Serial.println("MDNS responder started\n");
            }

            webServer.on("/", handleRoot);

            webServer.on("/inline", [&]() {
                webServer.send(200, "text/plain", "this workds well");
            });

            webServer.onNotFound(handleNotFound);
            webServer.begin();
            Serial.println("HTPP server started");
            delay(1000);
            webServer.handleClient();
            delay(2);
        }

        static void handleRoot() {
            webServer.send(200, "text/plain", "hello from esp32");
        }

        void handleNotFound() {
            String message = "File not found\n\n";
            message += "URI: ";
            message += webServer.uri();
            message += "\nMethod: ";
            message += (webServer.method() == HTTP_GET) ? "GET" : "POST";
        }
};

#endif