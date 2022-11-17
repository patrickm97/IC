#ifndef DIGITAL_SENSOR_HPP
#define DIGITAL_SENSOR_HPP

#include "isensor.hpp"

class DigitalSensor : public ISensor {
    public:
        DigitalSensor(String sensorId, vector<int> pins, unsigned long publishInterval): ISensor(sensorId, pins, publishInterval){
        }

        long readValue() {
            if (this->pins.size() > 0) {
                long value = 0;
                value = digitalRead(this->pins[0]);
                return value;
            }
            return -1;
        }
};

#endif