#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <iostream>
#include <iterator>
#include <list>
#include <vector>
using namespace std;
#include "isensor.hpp"

class Runner {
    private:
        vector<ISensor> sensors;
        vector<int> waitTimeArray;
    
    public:
        Runner() {
            for (int i = 0; i < sensors.size(); i++) {
                waitTimeArray.push_back(sensors[i].readValue());
            }
        }

        void loop() {
            cout << "runner loop";
        }
        void setInterval() {
            cout << "runner set interval";
        }
        void loadSensorsConfig() {
            cout << "runner load sensors config";
        }

};

#endif