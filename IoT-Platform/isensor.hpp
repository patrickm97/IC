#ifndef ISENSOR_HPP
#define ISENSOR_HPP

#include <iostream>
#include <iterator>
#include <vector>
using namespace std;

class ISensor {
    public:
        int tipo;
        vector<int> pins;
        String sensorId;
        unsigned long publishInterval = 0;
        unsigned long lastPublish = 0;
        
        virtual long readValue() = 0;
    
    ISensor(String sensorId, int tipo, vector<int> pins, unsigned long publishInterval)
    : sensorId(sensorId), tipo(tipo), pins(pins), publishInterval(publishInterval)
    {
    };
};

#endif