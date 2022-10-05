#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <iostream>
#include <iterator>
#include <list>
using namespace std;
#include "isensor.hpp"

class Runner {
    private:
        list<ISensor>::iterator sensors;
    
    public:
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