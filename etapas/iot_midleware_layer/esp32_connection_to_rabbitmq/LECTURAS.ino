long data_S_PH; 
long data_S_EC; 
long data_S_ORP; 
long data_S_O2A; 
long data_S_T;
long data_S_F; 

#include <PubSubClient.h>
#include <Ezo_uart.h>
#include <WiFi.h>
#include <SoftwareSerial.h>                           //we have to include the SoftwareSerial library, or else we can't use it
#define rx 16                                          //define what pin rx is going to be
#define tx 17                                          //define what pin tx is going to be
SoftwareSerial myserial(rx, tx);                      //define how the soft serial port is going to work

int s1 = 34;                                           //Arduino pin 6 to control pin S1
int s2 = 35;                                           //Arduino pin 5 to control pin S2
int s3 = 32;                                           //Arduino pin 4 to control pin S3
int port = 1;                                         //what port to open

const uint8_t bufferlen = 64;                         //total buffer size for the response_data array
char response_data[bufferlen];                        //character array to hold the response data from modules
String inputstring = "";                              //a string to hold incoming data from the PC

// create objects to represent the Modules you're connecting to
// they can accept hardware or software serial ports, and a name of your choice
Ezo_uart Module1(myserial, "S_O2A");
Ezo_uart Module2(myserial, "S_EC");
Ezo_uart Module3(myserial, "S_PH");
Ezo_uart Module4(myserial, "S_ORP");
Ezo_uart Module5(myserial, "S_T");
Ezo_uart Module6(myserial, "S_F");

// the modules are ordered in an array according to their position in the serial port expander
// so Modules[0] holds the module in port1, Modules[1] holds the module in port 2, etc
const uint8_t module_count = 6;                       //total size fo the Modules array
Ezo_uart Modules[module_count] = {                    //create an array to hold all the modules
  Module1, Module2, Module3, Module4, Module5, Module6
};

const char* ssid = "EL-PURGATORIO";
const char* password = "1128264024J";
const char* mqtt_server = "138.197.57.237"; 
const char* mqtt_user = "cerex2";
const char* mqtt_pass= "159753";

WiFiClient espClient;
PubSubClient client(espClient);
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}
void reconnect() {
  // Loop until we're reconnected
  Serial.println("In reconnect...");
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("cerex2", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

//TaskHandle_t Task2, Task3;
//int cuenta = 0;


void setup() {
  Serial.begin(115200);                                 //Set the hardware serial port to 9600
  myserial.begin(9600);                               //set baud rate for the software serial port to 9600
  initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
  client.setServer(mqtt_server,1883);
  inputstring.reserve(20);                            //set aside some bytes for receiving data from the 

  pinMode(s1, OUTPUT);                                //Set the digital pin as output
  pinMode(s2, OUTPUT);                                //Set the digital pin as output
  pinMode(s3, OUTPUT);                                //Set the digital pin as output


   // in order to use multiple circuits more effectively we need to turn off continuous mode and the *ok response
  for (uint8_t i = 0; i < module_count; i++) {        // loop through the modules
    open_port(i + 1);                                 // open the port
    Modules[i].send_cmd_no_resp("c,0");               //send the command to turn off continuous mode
                                                      //in this case we arent concerned about waiting for the reply
    delay(100);
    Modules[i].send_cmd_no_resp("*ok,0");             //send the command to turn off the *ok response
                                                      //in this case we wont get a reply since its been turned off
    delay(100);
    Modules[i].flush_rx_buffer();                     //clear all the characters that we received from the responses of the above commands
  }
  
  

}

void theread_for_read_data() {
   
  if (Serial.available() > 0) {                       //if we get data from the computer
    inputstring = Serial.readStringUntil(13);         //receive it until the carraige return delimiter
    port = parse_input(inputstring);                  //parse the data to either switch ports or send it to the circuit
    open_port(port);                                  //set the port according to the data we sent

    if (inputstring != "") {                          //if we have a command for the modules
      Modules[port - 1].send_cmd(inputstring, response_data, bufferlen); // send it to the module of the port we opened
      Serial.print(port);                             //print the modules port
      Serial.print("-");
      Serial.print(Modules[port - 1].get_name());     //print the modules name
      Serial.print(": ");
      Serial.println(response_data);                  //print the modules response
      response_data[0] = 0;                           //clear the modules response
      
    }
    else {
      Serial.print("Port is set to ");                //if were not sending a command, print the port
      Serial.println(port);
    }
  }

  for (uint8_t i = 0; i < module_count; i++) {        //loop through the modules and take a reading
    open_port(i + 1);
    print_reading(Modules[i]);
    Serial.print(" ");
  }
  Serial.println();

}


void print_reading(Ezo_uart &Module) {                //takes a reference to a Module
  //send_read() sends the read command to the module then converts the 
  //answer to a float which can be retrieved with get_reading()
  //it returns a bool indicating if the reading was obtained successfully
  if (Module.send_read()) {                           
    Serial.print(Module.get_name());                  //prints the module's name
    Serial.print(": ");
    Serial.print(Module.get_reading());               //prints the reading we obtained
    Serial.print("    ");
  }
}

uint8_t parse_input(String &inputstring) {                 //this function will decode the string (example 4:cal,1413)
  int colon = inputstring.indexOf(':');                  //find the location of the colon in the string
  if ( colon > 0) {                                       //if we found a colon
    String port_as_string = inputstring.substring(0, colon);  //extract the port number from the string and store it here
    inputstring = inputstring.substring(colon + 1);    //extract the message from the string and store it here
    return port_as_string.toInt();                     //convert the port number from a string to an int
  }
  else {                                              //if theres no colon
    return port;                                      //return the current port and dont modify the input string
  }
}

void open_port(uint8_t _port) {                                  //this function controls what port is opened on the serial port expander

  if (port < 1 || module_count > 8)_port = 1;                //if the value of the port is within range (1-8) then open that port. If it’s not in range set it to port 1
  uint8_t port_bits = _port - 1;

  digitalWrite(s1, bitRead(port_bits, 0));               //Here we have two commands combined into one.
  digitalWrite(s2, bitRead(port_bits, 1));               //The digitalWrite command sets a pin to 1/0 (high or low)
  digitalWrite(s3, bitRead(port_bits, 2));               //The bitRead command tells us what the bit value is for a specific bit location of a number
  delay(2);                                         //this is needed to make sure the channel switching event has completed
}

void loop(){
  theread_for_read_data();
  theread_for_send_data();
}

void theread_for_send_data(){
    
   
        data_S_O2A = Modules[0].get_reading();
        data_S_EC = Modules[1].get_reading();
        data_S_PH = Modules[2].get_reading();
        data_S_ORP = Modules[3].get_reading();
        data_S_T = Modules[4].get_reading();
        data_S_F = Modules[5].get_reading();
        char msg[8];
        
        if(!client.connected()) {
            reconnect();
        }
        
        sprintf(msg,"%i",data_S_O2A);
        client.publish("S_OA",msg);
        sprintf(msg,"%i",data_S_EC);
        client.publish("S_EC",msg);
        sprintf(msg,"%i",data_S_PH);
        client.publish("S_PH",msg);
        sprintf(msg,"%i",data_S_ORP);
        client.publish("S_ORP",msg);
        sprintf(msg,"%i",data_S_T);
        client.publish("S_T",msg);
        sprintf(msg,"%i",data_S_F);
        client.publish("S_O2G",msg);
}
