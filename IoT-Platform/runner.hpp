#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <iostream>
#include <iterator>
#include <list>
#include <vector>
using namespace std;
#include "isensor.hpp"
#include "configmqtt.hpp"

class Runner {
    private:
        vector<ISensor*> sensors;
        vector<int> waitTimeArray;
        ConfigMqtt &config;
    public:
        Runner(ConfigMqtt &config) : config(config) {
            
        }

        void addSensor(ISensor* sensor){
            sensors.push_back(sensor);
        }

        void loop() {
            for(int i=0;i<sensors.size();i++){
                if(millis() - sensors[i]->lastPublish >  sensors[i]->publishInterval){
                    long valorSensor = sensors[i]->readValue();
                    config.publish(sensors[i]->sensorId, String(valorSensor));
                    Serial.println("enviou sensor");
                    sensors[i]->lastPublish = millis();
                }
            }
        }
        void setInterval() {
            cout << "runner set interval";
        }
        void loadSensorsConfig() {
            cout << "runner load sensors config";
        }

};

#endif