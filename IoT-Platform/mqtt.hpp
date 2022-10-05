#ifndef MQTT_HPP
#define MQTT_HPP

#include "wificlient.hpp"
#include <iostream>
using namespace std;

class Mqtt {
    private:
        WifiClient &wifiClient;
    public:
        Mqtt(WifiClient &wifiClient) : wifiClient(wifiClient) {
        }
        void connectMQTT() {
            wifiClient.connectWiFi("10.42.0.1", "lab@iiot");
            cout << "connectmqtt";
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