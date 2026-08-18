#include <Arduino.h>
#include <future>
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include "../lib/io.cpp"
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

String loadConfig()
{
  int i = 0;
  if (!LittleFS.begin())
  {
    while (i < 3)
    {
      blink(1000);
      blink(1000);
      blink(1000);
      i = i + 1;
    }
    return "Failed to mount LittleFS";
  }

  // Check if file exists
  if (!LittleFS.exists("/config.txt"))
  {
    while (i < 3)
    {
      blink(500);
      blink(1000);
      blink(500);
      i = i + 1;
    }
    return "File /config.txt does NOT exist on filesystem!";
  }

  File file = LittleFS.open("/config.txt", "r");
  if (!file)
  {
    while (i < 3)
    {
      blink(50);
      blink(50);
      blink(50);
      blink(1000);
      i = i + 1;
    }
    return "Failed to open /config.txt for reading";
  }

  Serial.printf("Found /config.txt (%d bytes)\n", file.size());

  while (file.available())
  {
    String line = file.readStringUntil('\n');
    line.trim();
    if (line.startsWith("//") || line.length() == 0)
      continue;

    int sepIndex = line.indexOf('=');
    if (sepIndex == -1)
      continue;

    String key = line.substring(0, sepIndex);
    String value = line.substring(sepIndex + 1);

    if (key == "WIFI_SSID")
      WIFI_SSID = value;
    else if (key == "WIFI_PASSWORD")
      WIFI_PASSWORD = value;
    else if (key == "MQTT_BROKER_ADDRESS")
      MQTT_BROKER_ADRRESS = value;
    else if (key == "MQTT_PORT")
      MQTT_PORT = value.toInt();
    else if (key == "MQTT_CLIENT_ID")
      MQTT_CLIENT_ID = value;
    else if (key == "MQTT_USERNAME")
      MQTT_USERNAME = value;
    else if (key == "MQTT_PASSWORD")
      MQTT_PASSWORD = value;
    else if (key == "MQTT_PUBLISH_TOPIC")
      MQTT_PUBLISH_TOPIC = value;
    else if (key == "MQTT_SUBSCRIBE_TOPIC")
      MQTT_SUBSCRIBE_TOPIC = value;
  }

  file.close();
  return "";
}

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

  // waits for the next measurement
  delay(5000);
}
