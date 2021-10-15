#include <WiFi.h>
#include <PubSubClient.h>
#include <Ezo_uart.h>
#include <SoftwareSerial.h>    

long data_S_PH;
long data_S_EC;
long data_S_ORP;
long data_S_O2A;
long data_S_T;
long data_S_O2G;
long data_S_CO2;

//Configuración del tiempo para los hilos
unsigned long time_for_read_data;
unsigned long time_for_send_data;
unsigned long time_for_read_data_flag= 0;
unsigned long time_for_send_data_flag= 0;


//Configuración de los pines en los que estara conectado el expander port
#define rx 10                                         
#define tx 19                                         
SoftwareSerial myserial(rx, tx);  
int s1 = 36;                                           
int s2 = 39;                                           
int s3 = 34;                                           
int port = 1; //Puerto que se desea abrir del expander port

const uint8_t bufferlen = 32;                         
char response_data[bufferlen];                       
String inputstring = "";   

Ezo_uart Module1(myserial, "S_PH");
Ezo_uart Module2(myserial, "S_EC");
Ezo_uart Module3(myserial, "S_ORP");
Ezo_uart Module4(myserial, "S_O2A");
Ezo_uart Module5(myserial, "S_T");
Ezo_uart Module6(myserial, "S_O2G");
Ezo_uart Module7(myserial, "S_CO2");

const uint8_t module_count = 7;                         
Ezo_uart Modules[module_count] = {                    
  Module1, Module2, Module3, Module4, Module5, Module6, Module7
};


//Configuración de las credenciales para la conexión de red y servidor mqtt
const char* ssid = "FJD302";
const char* password = "hk10293 8";
const char* mqtt_server = "143.244.204.187"; 
const char* mqtt_user = "admin1";
const char* mqtt_pass= "admin1";


WiFiClient espClient;
PubSubClient client(espClient);

//Función para conectar el esp32 a la red de WIFI
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("ConectaNDO al WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

//Función para conectar el esp32 al servidor mqtt
void reconnect() {
    // Ciclo hasta que se conecte el esp32 a la red de WIFI.
  Serial.println("In reconnect...");
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("cerex", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


//Función para imprimir el dato que se obtuvo de los puertos
void print_reading(Ezo_uart &Module) {                
  if (Module.send_read()) {                           
    Serial.print(Module.get_name());                  
    Serial.print(": ");
    Serial.print(Module.get_reading());               
    Serial.print("    ");
  }
}

//Función para decodificar la cadena que se le envia al puerto rn otras palabras las instrucciones
uint8_t parse_input(String &inputstring) {                 
  int colon = inputstring.indexOf(':');                  
  if ( colon > 0) {                                       
    String port_as_string = inputstring.substring(0, colon);  
    inputstring = inputstring.substring(colon + 1);    
    return port_as_string.toInt();                     
  }
  else {                                              
    return port;                                      
  }
}

//Función que controla la apertura de los puertos del serial expand port
void open_port(uint8_t _port) {                                  

  if (port < 1 || module_count > 8)_port = 1;               
  uint8_t port_bits = _port - 1;

  digitalWrite(s1, bitRead(port_bits, 0));               
  digitalWrite(s2, bitRead(port_bits, 1));               
  digitalWrite(s3, bitRead(port_bits, 2));               
  delay(2);                                              
}

//Hilo para leer los datos de los sensores
void theread_for_read_data(){
    if(time_for_read_data==100){
        time_for_read_data_flag = time_for_read_data;
        if (Serial.available() > 0) {                       
            inputstring = Serial.readStringUntil(13);         
            port = parse_input(inputstring);                  
            open_port(port);                                  

            if (inputstring != "") {                         
                Modules[port - 1].send_cmd(inputstring, response_data, bufferlen); 
                Serial.print(port);                             
                Serial.print("-");
                Serial.print(Modules[port - 1].get_name());     
                Serial.print(": ");
                Serial.println(response_data);                  
                response_data[0] = 0;                           
            }
            else {
                Serial.print("El puerto esta ajustado a ");                
                Serial.println(port);
            }
        }

        for (uint8_t i = 0; i < module_count; i++) {        
            open_port(i + 1);
            print_reading(Modules[i]);
            Serial.print(" ");
        }
        Serial.println();
    }
}

//Hilo para enviar los datos al servidor
void theread_for_send_data(){
    if(time_for_send_data==200){
        time_for_send_data_flag = time_for_send_data;
        data_S_PH = Module[0].get_reading();
        data_S_EC = Module[1].get_reading();
        data_S_ORP = Module[2].get_reading();
        data_S_O2A = Module[3].get_reading();
        data_S_T = Module[4].get_reading();
        data_S_O2G = Module[5].get_reading();
        data_S_CO2 = Module[6].get_reading();
        char msg[8]
        if(!client.connected()) {
            reconnect();
        }
        sprintf(msg,"%i",data_S_PH);
        client.publish("S_PH",msg)
        sprintf(msg,"%i",data_S_EC);
        client.publish("S_EC",msg);
        sprintf(msg,"%i",data_S_ORP);
        client.publish("S_ORP",msg);
        sprintf(msg,"%i",data_S_O2A);
        client.publish("S_O2A",msg);
        sprintf(msg,"%i",data_S_T);
        client.publish("S_T",msg);
        sprintf(msg,"%i",data_S_O2G);
        client.publish("S_O2G",msg);
        sprintf(msg,"%i",data_S_CO2);
        client.publish("S_CO2",msg);

    }
}

//Configuraciones principales
void setup() {
    Serial.begin(115200);
    myserial.begin(9600);                               
    inputstring.reserve(20); 
    initWiFi();
    Serial.print("RRSI: ");
    Serial.println(WiFi.RSSI());
    client.setServer(mqtt_server,1883); 
    pinMode(s1, OUTPUT);                                
    pinMode(s2, OUTPUT);                                
    pinMode(s3, OUTPUT);

    //Calibración del serial port expander, verificar si da respuesta
    for (uint8_t i = 0; i < module_count; i++) {        
    open_port(i + 1);                                   
    Modules[i].send_cmd_no_resp("c,0");                 
                                                        
    delay(100);
    Modules[i].send_cmd_no_resp("*ok,0");             
                                                      
    delay(100);
    Modules[i].flush_rx_buffer();                     
  }
}


void loop() {
    time_for_send_data = millis();
    time_for_send_data = millis();
    theread_for_read_data();
    theread_for_send_data();
    
}

