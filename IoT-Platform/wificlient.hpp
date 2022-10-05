#ifndef WIFICLIENT_HPP
#define WIFICLIENT_HPP

#include <iostream>
#include <string>
#include <WiFi.h>

using namespace std;

class WifiClient {
    public:
        void connectWiFi(const char* ssid, const char* password) {
            WiFi.begin(ssid, password);
        }
};

#endif