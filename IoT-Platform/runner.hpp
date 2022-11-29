#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <iostream>
#include <vector>
#include "isensor.hpp"
#include "configmqtt.hpp"

class Runner {
    private:
        vector<ISensor*> sensors;
        ConfigMqtt &config;

    public:
        Runner(ConfigMqtt &config) : config(config) {
            config.setInterpretConfigCallback([this](byte* payload, unsigned int length){
                this->interpretCallback(payload,length);
            });
        }

        void addSensor(ISensor* sensor){
            sensors.push_back(sensor);
        }

        void setInterval(String sensorId, unsigned long interval) {
            for (int i = 0; i < sensors.size(); i++) {
                if (sensors[i]->sensorId == sensorId)
                    sensors[i]->publishInterval = interval;
            }
        }
        // .
        void loop() {
            for (int i = 0; i < sensors.size(); i++) {
                if (millis() - sensors[i]->lastPublish > sensors[i]->publishInterval) {
                    long valorSensor = sensors[i]->readValue();
                    config.publish(sensors[i]->sensorId, String(valorSensor));
                    sensors[i]->lastPublish = millis();
                }
            }
        }

        void interpretCallback(byte* payload, unsigned int length){
            Serial.println("Config chamado com:");
            // vai chamar add sensor
            // JSON recebido do Mqtt precisa do ID, intervalo, porta[], tipo (digital/analog)
            // if analog -> addSensor(new AnalogSensor...)
            // if digital -> addSensor(new DigitalSensor...)
            // concatenar topico dev/id
            /*  exemplo de formato, escrito no publish no terminal
                '
                {
                    'ID':'LDR',
                    'intervalo':'5000',
                    'pins':'12',
                    'tipo':'analog'
                }
                '
            */
        }
};

#endif