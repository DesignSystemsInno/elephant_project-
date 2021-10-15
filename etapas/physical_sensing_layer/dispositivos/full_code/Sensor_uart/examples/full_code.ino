#include <SoftwareSerial.h>
#include <PubSubClient.h>

#define ph_rx;                                          
#define ph_tx;
#define ec_rx; 
#define ec_tx;
#define orp_rx;
#define orp_tx;
#define oa_rx; 
#define oa_tx;
#define o2_rx; 
#define o2_tx;
#define flow_rx; 
#define flow_tx;
                    
unsigned long mqtt_time;
unsigned long ph_time;
unsigned long ec_time;
unsigned long orp_time;
unsigned long oa_time;
unsigned long o2_time;
unsigned long flow_time;
unsigned long mqtt_time_update;
unsigned long ph_time_update;
unsigned long ec_time_update;
unsigned long orp_time_update;
unsigned long oa_time_update;
unsigned long o2_time_update;
unsigned long flow_time_update;



const char* mqtt_server = ""; 
const char* mqtt_user = ""; 
const char* mqtt_password = "";

String inputString = "";                              
String sensorstring = "";                              
boolean input_string_complete = false;                
boolean sensor_string_complete = false;               

WiFiClient espClient;
SoftwareSerial ph_myserial(ph_rx, ph_tx);
SoftwareSerial ec_myserial(ec_rx, ec_tx);
SoftwareSerial orp_myserial(orp_rx, orp_tx);
SoftwareSerial oa_myserial(oa_rx, oa_tx);
SoftwareSerial oa_myserial(o2_rx, o2_tx);
SoftwareSerial flow_myserial(flow_rx, flow_tx);
PubSubClient client(espClient);

void reconnect(){
    Serial.println("In reconnect ...");
    while(!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("Arduino_Gas", mqtt_user, mqtt_password)) {
            Serial.println("connected");
        }else{
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void setup(){
    Serial.begin(9600);
    ph_myserial.begin(9600);
    ec_myserial.begin(9600);
    inputString.reserve(10);
    sensorstring.reserve(30);
    client.setServer(mqtt_server, "port");

}

void serialEvent(){
    inputstring = Serial.readString(13);
    input_string_complete = true;
}

void loop(){
    mqtt_time = millis();
    ph_time = millis();
    ec_time = millis();

}


void send_data_server_qtt(){
    if(mqtt_time-mqtt_time_update == 100){
        mqtt_time_update = mqtt_time;
        char msg[8];
        if (!client.connected()) {
            reconnect();
        }
        sprintf(msg,"%i",sensorstring);
        client.publish(msg);
    }
   

}

void data_sensor_ph(){
    if(ph_time-ph_time_update == 200){
        ph_time_update = ph_time;
        if(input_string_complete){
            ph_myserial.print(inputString);
            ph_myserial.print('\r');
            inputString = "";
            input_string_complete = false;
        }
    }
    if(ph_myserial.available() > 0){
        char inchar = (char) ph_myserial.read();
        sensorstring += inchar;                           
        if(inchar== '\r'){
            sensor_string_complete = true;                      
        }
    }
    if(sensor_string_complete){
        Serial.println(sensorstring);
        sensorstring = "";
        sensor_string_complete = false;
    }

    
}

void data_sensor_ec(){
    

     if(ec_time-ec_time_update == 300){
        ec_time_update = ec_time;
        if(input_string_complete){
            ec_myserial.print(inputString);
            ec_myserial.print('\r');
            inputString = "";
            input_string_complete = false;
        }
    }
    if(ec_myserial.available() > 0){
        char inchar = (char) ec_myserial.read();
        sensorstring += inchar;                           
        if(inchar== '\r'){
            sensor_string_complete = true;                      
        }
    }
    if(sensor_string_complete){
        Serial.println(sensorstring);
        sensorstring = "";
        sensor_string_complete = false;
    }

}

void data_sensor_orp(){
     if(orp_time-orp_time_update == 400){
        orp_time_update = orp_time;
        if(input_string_complete){
            orp_myserial.print(inputString);
            orp_myserial.print('\r');
            inputString = "";
            input_string_complete = false;
        }
    }
    if(orp_myserial.available() > 0){
        char inchar = (char) orp_myserial.read();
        sensorstring += inchar;                           
        if(inchar== '\r'){
            sensor_string_complete = true;                      
        }
    }
    if(sensor_string_complete){
        Serial.println(sensorstring);
        sensorstring = "";
        sensor_string_complete = false;
    }

}

void data_sensor_oa(){
     if(oa_time-oa_time_update == 400){
        oa_time_update = oa_time;
        if(input_string_complete){
            oa_myserial.print(inputString);
            oa_myserial.print('\r');
            inputString = "";
            input_string_complete = false;
        }
    }
    if(oa_myserial.available() > 0){
        char inchar = (char) oa_myserial.read();
        sensorstring += inchar;                           
        if(inchar== '\r'){
            sensor_string_complete = true;                      
        }
    }
    if(sensor_string_complete){
        Serial.println(sensorstring);
        sensorstring = "";
        sensor_string_complete = false;
    }

}

void data_sensor_o2(){
     if(o2_time-o2_time_update == 500){
        o2_time_update = o2_time;
        if(input_string_complete){
            o2_myserial.print(inputString);
            o2_myserial.print('\r');
            inputString = "";
            input_string_complete = false;
        }
    }
    if(o2_myserial.available() > 0){
        char inchar = (char) o2_myserial.read();
        sensorstring += inchar;                           
        if(inchar== '\r'){
            sensor_string_complete = true;                      
        }
    }
    if(sensor_string_complete){
        Serial.println(sensorstring);
        sensorstring = "";
        sensor_string_complete = false;
    }

}

void data_sensor_flow(){
     if(flow_time-flow_time_update == 500){
        flow_time_update = flow_time;
        if(input_string_complete){
            flow_myserial.print(inputString);
            flow_myserial.print('\r');
            inputString = "";
            input_string_complete = false;
        }
    }
    if(flow_myserial.available() > 0){
        char inchar = (char) flow_myserial.read();
        sensorstring += inchar;                           
        if(inchar== '\r'){
            sensor_string_complete = true;                      
        }
    }
    if(sensor_string_complete){
        Serial.println(sensorstring);
        sensorstring = "";
        sensor_string_complete = false;
    }

}




