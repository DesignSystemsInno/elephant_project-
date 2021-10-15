#include "Arduino.h"
#include "Sensor_uart.h"
#include <SoftwareSerial.h>

int rx;
int tx;
String inputString = "";
String sensorString = "";  
boolean input_string_complete = false;
boolean sensor_string_complete = false; 
SoftwareSerial myserial(rx, tx);

Sensor::Sensor(int rx, int tx){
    this -> rx = rx;
    this -> tx = tx;
    sensorString.reserve(30);
    myserial.begin(9600);
    
}

void Sensor::serialEvent(){
    inputString = Serial.readStringUntil(13);  
    input_string_complete = true;
}

void Sensor::dataSensor(){
    if(input_string_complete){
        myserial.print(inputString);
        myserial.print("\r");
        inputString = "";
        input_string_complete = false;

    }

    if(myserial.available()>0){
        char inchar = (char)myserial.read();
        sensorString +=inchar;
        if(inchar == '\r'){
            sensor_string_complete = true;
        }
    }

    if (sensor_string_complete){
        Serial.println(sensorString);

        sensorString = "";
        sensor_string_complete = false;
    }
}
