#ifndef ISENSOR_HPP
#define ISENSOR_HPP

#include <iostream>
#include <iterator>
#include <list>
using namespace std;

class ISensor {
    public:
        int tipo;
        list<int>::iterator pins;
        string sensorId;
        
        virtual float readValue() = 0;
        
        int readInterval() {
            this->tipo = 5;
            return this->tipo;
        }
};

#endif