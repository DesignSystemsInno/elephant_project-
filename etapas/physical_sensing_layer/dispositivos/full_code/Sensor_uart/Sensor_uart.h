#ifndef Sensor_uart.h
#define Sensor_uart.h
#include "Arduino.h"
class Sensor{
    public:
        Sensor(int rx, int tx);
        void serialEvent();
        void dataSensor();
        int rx;
        int tx;
    
};
#endif
