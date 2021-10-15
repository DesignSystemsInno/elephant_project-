#include <WiFi.h>
#include <PubSubClient.h>

const char* ssi = "";
const char* password_wifi = "";
const char* mqtt_server = ""; 
const char* mqtt_user = "";
const char* mqtt_pass= "";

PubSubClient client(espClient);

void initWifi(){
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
    if (client.connect("", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup(){
    Serial.begin(115200);
    initWifi();
    Serial.print("RRSI: ");
    Serial.println(WiFi.RSSI());
}

void looop(){

}