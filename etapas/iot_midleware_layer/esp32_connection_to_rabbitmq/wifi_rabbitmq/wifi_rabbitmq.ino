#include <Ezo_uart.h>

#include <WiFi.h>
#include <PubSubClient.h>
long randNumber;
// Replace with your network credentials (STATION)
const char* ssid = "TIGO-520B";
const char* password = "4D9697506784";
const char* mqtt_server = "143.244.204.187"; 
const char* mqtt_user = "admin1";
const char* mqtt_pass= "admin1";

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

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(115200);
  initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
  client.setServer(mqtt_server,1883);
}

void loop() {
  randNumber = random(300);
  
  char msg[8];
  if (!client.connected()) {
    reconnect();
  }
   randNumber = random(300);
  sprintf(msg,"%i",randNumber); 
  client.publish("S_N", msg);
   randNumber = random(300);
  sprintf(msg,"%i",randNumber); 
  client.publish("S_NL", msg);
   randNumber = random(300);
  sprintf(msg,"%i",randNumber); 
  client.publish("S_H", msg);
   randNumber = random(300);
  sprintf(msg,"%i",randNumber); 
  client.publish("S_CO2", msg);
   randNumber = random(300);
  sprintf(msg,"%i",randNumber); 
  client.publish("S_K", msg);
   randNumber = random(300);
  sprintf(msg,"%i",randNumber); 
  client.publish("S_O2", msg);
   randNumber = random(300);
  sprintf(msg,"%i",randNumber); 
  client.publish("S_F", msg);
   randNumber = random(300);
  sprintf(msg,"%i",randNumber); 
  client.publish("S_P", msg);
   randNumber = random(300);
  sprintf(msg,"%i",randNumber); 
   client.publish("S_OA", msg);
    randNumber = random(300);
   sprintf(msg,"%i",randNumber); 
  client.publish("S_EC", msg);
   randNumber = random(300);
  sprintf(msg,"%i",randNumber); 
   client.publish("S_T", msg);
    randNumber = random(300);
   sprintf(msg,"%i",randNumber); 
  client.publish("S_ORP", msg);
   randNumber = random(300);
  sprintf(msg,"%i",randNumber); 
   client.publish("S_PH", msg);
  
  Serial.println(msg);

  delay(1000);
  // put your main code here, to run repeatedly:
}
