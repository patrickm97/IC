#ifndef ANALOG_SENSOR_HPP
#define ANALOG_SENSOR_HPP

#include "isensor.hpp"

class AnalogSensor : public ISensor {
    public:
        AnalogSensor(String sensorId, vector<int> pins, unsigned long publishInterval): ISensor(sensorId, pins, publishInterval) {
        }

        long readValue() {
            if (this->pins.size() > 0) {
                long value = 0;
                value = analogRead(this->pins[0]);
                value = ::map(value, 4095, 2500, 0, 100);
                return value;
            }
            return -1;
        }
};

#endif