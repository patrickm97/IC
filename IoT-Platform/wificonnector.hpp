#ifndef WIFICONNECTOR_HPP
#define WIFICONNECTOR_HPP

#include <iostream>
#include <string>
#include <WiFi.h>
#include <WiFiClient.h>
#include <regex>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "configParams.hpp"
#include "wifiserverconfigurator.hpp"

using namespace std;

class WifiConnector {
    private:
        WebServer webServer;
        char serverSsid[100];
        char serverPassword[20];
        regex ipRegex;
        LocalStorage& storage;
        WifiServerConfigurator wifiServerConfigurator; // don't use & when class owns this object
        
    public:
        WifiConnector(LocalStorage& storage) : storage(storage), wifiServerConfigurator(storage) {
            int rcode = rand() % 10000;
            snprintf(this->serverSsid, 100, "%s%05d", "IoTAgro",rcode);
            snprintf(this->serverPassword, 20, "iot");
            this->ipRegex = "\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\:\\d{1,4}";
        }

        bool isDeviceConfigured() {
            return this->wifiServerConfigurator.isDeviceConfigured();
        }

        void connectWifiServer() {
            Serial.println();
            Serial.println();
            Serial.print("Connecting to ");
            Serial.println(serverSsid);

            WiFi.softAP(serverSsid, serverPassword);
            WiFi.enableAP(true);
            IPAddress myIP = WiFi.softAPIP();
            Serial.print("AP IP address: ");
            Serial.println(myIP);

            setupWebServer();
        }

        void connectWiFiClient(const char* ssid, const char* password) {
            Serial.print("Connecting to network: ");
            Serial.print(ssid);
            Serial.print(", password: ");
            Serial.println(password);
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
        }

        
        // WebServer methods
        void setupWebServer() {
            if (MDNS.begin("esp")) {
                Serial.println("MDNS responder started");
            }

            webServer.on("/", [this]() { this->handleRoot(); } );

            webServer.on("/form", [&]() {
                ConfigParams config;
                
                config.deviceId = webServer.arg("deviceId");
                config.ssid = webServer.arg("ssid");
                config.password = webServer.arg("password");
                config.mqttIP = webServer.arg("mqttIP");
                config.mqttSocket = webServer.arg("mqttSocket");
                config.topic = webServer.arg("topic");

                webServer.send(200, "text/html", "<p>Data registered</p><br>");
                
                storage.saveConfig(config);
                Serial.println("Network configuration saved");
                Serial.println("Restarting device...");
                delay(2000);
                ESP.restart();
            });

            webServer.onNotFound([this]() { this->handleNotFound(); });
            webServer.begin();
            Serial.println("HTPP server started");
            delay(1000);
        }

        void handleRoot() {
            String s;
            s = "<html><header>Device parameters form</header><br><p>Fill out the form</p>";
            s += "<form action='/form'>";
            s += "<input placeholder='device ID' type='text' required='true' name='deviceId'><br><br>";
            s += "<input placeholder='wi-fi ssid' type='text' required='true' name='ssid'><br><br>";
            s += "<input placeholder='password' type='password' required='true' name='password'><br><br>";
            s += "<input placeholder='MQTT IP' type='text' required='true' name='mqttIP'><br><br>";
            s += "<input placeholder='MQTT Socket' type='text' required='true' name='mqttSocket'><br><br>";
            s += "<input placeholder='MQTT Topic' type='text' required='true' name='topic'><br><br>";
            s += "<input type='submit' value='send'/>";
            s += "</form>";
            s += "</html>";
            webServer.send(200, "text/html",s); 
        }

        void handleNotFound() {
            String message = "File not found\n\n";
            message += "URI: ";
            message += webServer.uri();
            message += "\nMethod: ";
            message += (webServer.method() == HTTP_GET) ? "GET" : "POST";
        }

        void handleWebServerClient() {
            webServer.handleClient();
        }
};

#endif