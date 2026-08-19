#include <Arduino.h>
#include <future>
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include "../lib/io.cpp"
#include "../lib/helpers.cpp"
#include "../lib/functions.cpp"
#include "../lib/mqtt.cpp"

String WIFI_SSID = "";
String WIFI_PASSWORD = "";
String MQTT_BROKER_ADRRESS = "";
int MQTT_PORT = 1883;
String MQTT_CLIENT_ID = "";
String MQTT_USERNAME = "";
String MQTT_PASSWORD = "";
String MQTT_PUBLISH_TOPIC = "";
String MQTT_SUBSCRIBE_TOPIC = "";
int SLEEP_TIME_MINUTES = 0;

void setup()
{
  list = new float[listlength];
  Serial.begin(9600);       // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input

  pinMode(ledPin, OUTPUT); // Sets the ledPin as an Output

  Serial.println("Trying to read the /data/config.txt file");

  String configError = loadConfig();
  if (configError.length() > 0)
  {
    while (true)
    {
      Serial.println("Error reading config.txt: " + configError);
      blink(100, 500);
    }
  }
  Serial.println("Read /data/config.txt file");

  const bool wifiConnected = wifiConnect(WIFI_SSID, WIFI_PASSWORD);
  if (wifiConnected == false)
    while (true)
    {
      blink(5000);
    }

  const bool mqttConnected = connectToMQTT(
      MQTT_BROKER_ADRRESS.c_str(),
      MQTT_PORT,
      MQTT_CLIENT_ID.c_str(),
      MQTT_USERNAME.c_str(),
      MQTT_PASSWORD.c_str(),
      MQTT_SUBSCRIBE_TOPIC.c_str());
  if (mqttConnected == false)
    while (true)
    {
      blink(2500);
    }
}

void loop()
{
  // takes the measurements and returns the value
  const float measurement = measure();

  sendToMQTT(MQTT_PUBLISH_TOPIC.c_str(), measurement);

  if (SLEEP_TIME_MINUTES > 0)
  {
    Serial.printf("Entering deep sleep for %d minute(s)...\n", SLEEP_TIME_MINUTES);
    // Convert minutes to microseconds (1 min = 60,000,000 us)
    ESP.deepSleep((uint64_t)SLEEP_TIME_MINUTES * 60 * 1000000ULL);
  }
  else
  {
    // waits for the next measurement if sleep mode is 0
    Serial.printf("No sleep time given, so i continue in 5 sec!");
    delay(5000);
  }
}
