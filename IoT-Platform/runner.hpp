#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <iostream>
#include <vector>
#include "isensor.hpp"
#include "analogSensor.hpp"
#include "digitalSensor.hpp"

class Runner {
    private:
        vector<ISensor*> sensors;
        ConfigMqtt &config;

    public:
        Runner(ConfigMqtt &config) : config(config) {
            config.setInterpretConfigCallback([this](byte* payload, unsigned int length) {
                this->interpretCallbackAdd(payload,length);
            });
            config.setInterpretOutCallback([this](byte* payload, unsigned int length) {
                this->interpretCallbackOut(payload,length);
            });
        }

        void addSensor(ISensor* sensor, String type){
            sensors.push_back(sensor);
            Serial.print(type);
            Serial.println(" sensor added successfully!");
            Serial.print("SensorId: ");
            Serial.print(sensor->sensorId);
            Serial.print(", Pins: ");
            Serial.print(sensor->pins[0]);
            Serial.print(", PublishInterval: ");
            Serial.println(sensor->publishInterval);
        }
        
        void loop() {
            for (int i = 0; i < sensors.size(); i++) {
                if (millis() - sensors[i]->lastPublish > sensors[i]->publishInterval) {
                    long valorSensor = sensors[i]->readValue();
                    config.publish(sensors[i]->sensorId, String(valorSensor));
                    sensors[i]->lastPublish = millis();
                }
            }
        }

        void interpretCallbackAdd(byte* payload, unsigned int length) {
            /*  JSON format mosquitto_pub example
                '{
                    "id":"LDR",
                    "interval":5000,
                    "pins":[12],
                    "type":"analog"
                }'
            */
            char json[length+1];
            for (int i = 0; i < length; i++) {
                char c = (char)payload[i];
                json[i] = c;
            }
            Serial.println(json);

            DynamicJsonDocument doc(1024);
            deserializeJson(doc, json);
            
            String sensorId = doc["id"];
            int pin = doc["pins"][0];
            unsigned long publishInterval = doc["interval"];
            String type = doc["type"];
            vector<int> pins = {pin};

            if (type.equals("analog"))
                addSensor(new AnalogSensor(sensorId, pins, publishInterval), type);
            
            if (type.equals("digital"))
                addSensor(new DigitalSensor(sensorId, pins, publishInterval), type);
        }

        void interpretCallbackOut(byte* payload, unsigned int length) {
            /*  JSON format mosquitto_pub example 
                '{
                    "pin":13,
                    "signal":1,
                    "type":"digital"
                }'
            */
            char json[length+1];
            for (int i = 0; i < length; i++) {
                char c = (char)payload[i];
                json[i] = c;
            }
            Serial.println(json);

            DynamicJsonDocument doc(1024);
            deserializeJson(doc, json);
            
            int pin = doc["pin"];
            int signal = doc["signal"];
            String type = doc["type"];

            // ESP32 needs to use the pin 16 for analogWrite: ESP32's GPIO16
            if (type.equals("analog") && pin == 16) {
                const int freq = signal;
                const int ledChannel = 0;
                const int resolution = 8;

                ledcSetup(ledChannel, freq, resolution);
                ledcAttachPin(pin, ledChannel);

                // increase the LED brightness
                for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
                    ledcWrite(ledChannel, dutyCycle);
                    delay(15);
                }

                // decrease the LED brightness
                for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
                    ledcWrite(ledChannel, dutyCycle);   
                    delay(15);
                }
            }
            
            if (type.equals("digital") && (signal == 0 || signal == 1)) {
                pinMode(pin, OUTPUT);
                digitalWrite(pin, signal);
                Serial.print("Digitalwrite on pin ");
                Serial.print(pin);
                Serial.print(", signal: ");
                Serial.println(signal);
            }
        }
};

#endif