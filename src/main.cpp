#include <Arduino.h>
#include <future>
#include <ESP8266WiFi.h>
#include "../lib/io.cpp"
#include "../lib/functions.cpp"
#include "../lib/mqtt.cpp"


String WIFI_SSID = "Bing Pot!";
String WIFI_PASSWORD = "LargeCow4298!";

const char MQTT_BROKER_ADRRESS[] = "192.168.2.2";  // CHANGE TO MQTT BROKER'S ADDRESS
const int MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "SILOFILLING-esp8266-001";  // CHANGE IT AS YOU DESIRE
const char MQTT_USERNAME[] = "";                        // CHANGE IT IF REQUIRED, empty if not required
const char MQTT_PASSWORD[] = "";                        // CHANGE IT IF REQUIRED, empty if not required
// const char MQTT_USERNAME[] = "hass";                        // CHANGE IT IF REQUIRED, empty if not required
// const char MQTT_PASSWORD[] = "$7$101$DRIk6u3DnN6bsOpy$XxqmszcLGuzk4qbnTpw4Jh+zynKq2Y5wu5SkSZDMD470OvUC8ulHJyZFl5DI9uD6DyVvCiOA+Ly6qUXCeQCCyA==";                        // CHANGE IT IF REQUIRED, empty if not required

// The MQTT topics that ESP8266 should publish/subscribe
const char MQTT_PUBLISH_TOPIC[] = "SILOFILLING-esp8266-001/loopback";    // CHANGE IT AS YOU DESIRE
const char MQTT_SUBSCRIBE_TOPIC[] = "SILOFILLING-esp8266-001/loopback";  // CHANGE IT AS YOU DESIRE


void setup()
{
  list = new float[listlength];
  Serial.begin(9600);       // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input

  pinMode(ledPin, OUTPUT); // Sets the ledPin as an Output

  const bool wifiConnected = wifiConnect(WIFI_SSID, WIFI_PASSWORD);
  if (wifiConnected == false)
    while (true)
    {
      blink(5000);
    }

  const bool mqttConnected = connectToMQTT(MQTT_BROKER_ADRRESS, MQTT_PORT, MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD, MQTT_SUBSCRIBE_TOPIC);
    if (mqttConnected == false)
    while (true)
    {
      blink(2500);
    }
}

void loop()
{
  //takes the measurements and returns the value
  const float measurement = measure();


  sendToMQTT(MQTT_PUBLISH_TOPIC, measurement);

  //waits for the next measurement
  delay(5000);
}
