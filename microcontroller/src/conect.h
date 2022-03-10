// #ifndef conect_h
// #define conect_h
#include <WiFi.h>
#include <PubSubClient.h>
/////////////////// SETTINGS /////////////////////////////
const char* ssid = "Mi Note 10 Lite";
const char* password = "i6704407";
// const char* ssid = "Mayontak";
// const char* password = "ak6704407";

// MQTT
const char* mqtt_server = "wqtt.ru";
const int mqtt_port = 5031;
const char* mqtt_user = "student";
const char* mqtt_password = "student";


// RELAY
const String relay_topic = "leds";
//const int RELAY = 16;

/////////////////////////////////////////////////////////



WiFiClient espClient;
PubSubClient client(espClient);

bool relay_on = false;


void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
}


  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266-" + WiFi.macAddress();
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password) ) {
      Serial.println("connected");
      
      client.subscribe( (relay_topic + "/#").c_str() );

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
    }
