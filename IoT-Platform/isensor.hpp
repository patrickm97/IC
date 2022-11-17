#ifndef ISENSOR_HPP
#define ISENSOR_HPP

#include <iostream>
#include <vector>
using namespace std;

class ISensor {
    public:
        vector<int> pins;
        String sensorId;
        unsigned long publishInterval;
        unsigned long lastPublish;
        
        virtual long readValue() = 0;
        
        ISensor(String sensorId, vector<int> pins, unsigned long publishInterval) {
            this->sensorId = sensorId;
            this->pins = pins;
            this->publishInterval = publishInterval;
            this->lastPublish = 0;
        }
};

#endif