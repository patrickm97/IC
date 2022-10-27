#ifndef WIFICONNECTOR_HPP
#define WIFICONNECTOR_HPP

#include <iostream>
#include <string>
#include <WiFi.h>
#include <WiFiClient.h>
#include <regex>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "parametrosConfig.hpp"
#include "wifiserverconfigurator.hpp"

using namespace std;

class WifiConnector {
    private:
        //WiFiClient wifiClient;
        WebServer webServer;
        char serverSsid[100];
        char serverPassword[20];
        WiFiServer wifiServer;
        regex ipRegex;
        LocalStorage& storage;
        WifiServerConfigurator wifiServerConfigurator; // don't use & when class owns this object
        
    public:
        // constructor
        // : webServer(80)
        WifiConnector(LocalStorage& storage) : wifiServer(), storage(storage), wifiServerConfigurator(storage) {
            
            int rcode = rand() % 10000;
            snprintf(this->serverSsid, 100, "%s%05d", "IoTAgro",rcode);
            snprintf(this->serverPassword, 20, "iot");
            this->ipRegex = "\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\:\\d{1,4}";
        }

        bool isDeviceConfigured() {
            return this->wifiServerConfigurator.isDeviceConfigured();
        }

        /*
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

            if (number_of_networks == 0)
                Serial.println("No Wi-Fi network found");
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
        */
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

        // void connectWiFiClient(const char* ssid, const char* password) {
        //     Serial.print("Connecting to network: ");
        //     Serial.println(ssid);
        //     WiFi.mode(WIFI_STA);
        //     WiFi.begin(ssid, password);
            
        //     // wait for device to connect to network
        //     while (WiFi.status() != WL_CONNECTED) {
        //         delay(100);
        //         Serial.print(".");
        //     }

        //     // when connected, display IP address
        //     Serial.println("Connected!");
        //     Serial.print("IP address: ");
        //     Serial.println(WiFi.localIP());
        //     this->connectedToWifi = true;
        //     setupWebServer();
        //     delay(1000);
        // }

        // WiFiClient getWifiClient() {
        //     return this->client;
        // }
        
        // WebServer methods
        void setupWebServer() {
            if (MDNS.begin("esp")) {
                Serial.println("MDNS responder started");
            }

            webServer.on("/", [this]() { this->handleRoot(); } );

            webServer.on("/form", [&]() {
                ConfigParams config;
                config.mqttHost = webServer.arg("mqttHost");
                String deviceId = webServer.arg("deviceId");
                String ssid = webServer.arg("ssid");
                String password = webServer.arg("password");
                String mqttHost = webServer.arg("mqttHost");
                String mqttPass = webServer.arg("mqttPass");
                String topic = webServer.arg("topic");

                webServer.send(200, "text/html", "<p>Data registered</p><br>");
                
                storage.saveConfig(config);
                // RESETAR O ESP
                ESP.restart();
            });

            webServer.onNotFound([this]() { this->handleNotFound(); });
            webServer.begin();
            Serial.println("HTPP server started");
            delay(1000);
        }

        void handleRoot() {
            String s;
            s = "<html><header>Hello</header><br><p>How are you</p>";
            s += "<form action='/form'>";
            s += "<input placeholder='device ID' type='text' required='true' name='deviceId'>";
            s += "<input placeholder='ssid' type='text' required='true' name='ssid'>";
            s += "<input placeholder='senha' type='password' required='true' name='password'>";
            s += "<input placeholder='mqtt host' type='text' required='true' name='mqttHost'>";
            s += "<input placeholder='mqtt password' type='password' required='false' name='mqttPass'>";
            s += "<input placeholder='mqtt topic' type='text' required='true' name='topic'><br>";
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