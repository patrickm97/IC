#ifndef ANALOG_SENSOR_HPP
#define ANALOG_SENSOR_HPP

#include "isensor.hpp"

class AnalogSensor : public ISensor{

    public:
    AnalogSensor(String sensorId, int tipo, vector<int> pins, unsigned long publishInterval): ISensor(sensorId, tipo, pins, publishInterval){};    

    long readValue() {
        if (this->pins.size() > 0) {
            long value = 0;
            Serial.println(this->pins[0]);
            value = analogRead(this->pins[0]);
            value = ::map(value, 4095, 2500, 0, 100);
            return value;
        }
        return -1;
    };
};

#endif