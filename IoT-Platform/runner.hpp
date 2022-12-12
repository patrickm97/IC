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

        void interpretCallbackAdd(byte* payload, unsigned int length){
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

            // vai chamar add sensor
            // JSON recebido do Mqtt precisa do ID, intervalo, porta[], tipo (digital/analog)
            // if analog -> addSensor(new AnalogSensor...)
            // if digital -> addSensor(new DigitalSensor...)
            // concatenar topico dev/id
            /*  exemplo de formato, escrito no mosquitto_pub no terminal
                '{
                    "id":"LDR",
                    "interval":5000,
                    "pins":[12],
                    "typo":"analog"
                }'
            */
        }

        void interpretCallbackOut(byte* payload, unsigned int length){
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

            // vai chamar send signal
            // JSON recebido do Mqtt precisa do ID, intervalo, porta[], tipo (digital/analog)
            // if analog -> addSensor(new AnalogSensor...)
            // if digital -> addSensor(new DigitalSensor...)
            // concatenar topico dev/id
            /*  exemplo de formato, escrito no mosquitto_pub no terminal
                '{
                    "id":"LDR",
                    "interval":5000,
                    "pins":[12],
                    "typo":"analog"
                }'
            */
        }
};

#endif