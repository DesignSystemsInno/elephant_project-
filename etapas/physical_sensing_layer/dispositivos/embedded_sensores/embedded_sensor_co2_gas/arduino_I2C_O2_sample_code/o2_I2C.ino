//This code will work on an Arduino Uno and Mega
//This code was written to be easy to understand.
//Modify this code as you see fit.
//This code will output data to the Arduino serial monitor.
//Type commands into the Arduino serial monitor to control the EZO o2 sensor.
//This code was written in the Arduino 1.8.12 IDE
//This code was last tested 6/2020


#include <Wire.h>                //enable I2C.
#define address 108              //default I2C address number for EZO o2 sensor.



char computerdata[20];           //we make a 20 byte character array to hold incoming data from a pc/mac/other.
byte received_from_computer = 0; //we need to know how many characters have been received.
byte serial_event = false;       //a flag to signal when data has been received from the pc/mac/other.
byte code = 0;                   //used to hold the I2C response code.
char o2_data[20];                //we make a 20-byte character array to hold incoming data from the o2 sensor.
byte in_char = 0;                //used as a 1 byte buffer to store inbound bytes from the o2 sensor.
byte i = 0;                      //counter used for o2_data array.
int time_ = 600;                 //used to set the delay needed to process the command sent to the EZO o2 sensor.
float o2_float;                  //float var used to hold the value of the o2.


void setup()                    //hardware initialization.
{
  Serial.begin(9600);           //enable serial port.
  Wire.begin();                 //enable I2C port.
}


void serialEvent() {                                                              //this interrupt will trigger when the data coming from the serial monitor(pc/mac/other) is received.
  received_from_computer = Serial.readBytesUntil(13, computerdata, 20);           //we read the data sent from the serial monitor(pc/mac/other) until we see a <CR>. We also count how many characters have been received.
  computerdata[received_from_computer] = 0;                                       //stop the buffer from transmitting leftovers or garbage.
  serial_event = true;                                                            //set the serial event flag.
}


void loop() {                                                                     //the main loop.
  if (serial_event == true) {                                                     //if a command was sent to the EZO device.
    for (i = 0; i <= received_from_computer; i++) {                               //set all char to lower case, this is just so this exact sample code can recognize the "sleep" command.
      computerdata[i] = tolower(computerdata[i]);                                 //"Sleep" ≠ "sleep"
    }
    i = 0;                                                                        //reset i, we will need it later


    Wire.beginTransmission(address);                                              //call the circuit by its address number.
    Wire.write(computerdata);                                                     //transmit the command that was sent through the serial port.
    Wire.endTransmission();                                                       //end the I2C data transmission.


    if (strcmp(computerdata, "sleep") != 0) {                                     //if the command that has been sent is NOT the sleep command, wait the correct amount of time and request data.
      //if it is the sleep command, we do nothing. Issuing a sleep command and then requesting data will wake the o2 sensor.

      delay(time_);                                                               //wait the correct amount of time for the circuit to complete its instruction.

      Wire.requestFrom(address, 20, 1);                                           //call the circuit and request 20 bytes (this may be more than we need)
      code = Wire.read();                                                         //the first byte is the response code, we read this separately.

      switch (code) {                       //switch case based on what the response code is.
        case 1:                             //decimal 1.
          Serial.println("Success");        //means the command was successful.
          break;                            //exits the switch case.

        case 2:                             //decimal 2.
          Serial.println("Failed");         //means the command has failed.
          break;                            //exits the switch case.

        case 254:                           //decimal 254.
          Serial.println("Pending");        //means the command has not yet been finished calculating.
          break;                            //exits the switch case.

        case 255:                           //decimal 255.
          Serial.println("No Data");        //means there is no further data to send.
          break;                            //exits the switch case.
      }



      while (Wire.available()) {              //are there bytes to receive.
        in_char = Wire.read();                //receive a byte.
        o2_data[i] = in_char;                 //load this byte into our array.
        i += 1;                               //incur the counter for the array element.
        if (in_char == 0) {                   //if we see that we have been sent a null command.
          i = 0;                              //reset the counter i to 0.
          break;                              //exit the while loop.
        }
      }

      Serial.println(o2_data);                //print the data
      serial_event = false;                   //reset the serial event flag
      //o2_float = atof(o2_data);             //Uncomment this line if you want to take the o2 value and convert it into a floating point number.
      o2_data[0] = 0;                         //this stops the code from printing data from the last reading
    }
  }
}
