#pragma once
#include <Arduino.h>
#include <LittleFS.h>

extern String WIFI_SSID;
extern String WIFI_PASSWORD;
extern String MQTT_BROKER_ADRRESS;
extern int MQTT_PORT;
extern String MQTT_CLIENT_ID;
extern String MQTT_USERNAME;
extern String MQTT_PASSWORD;
extern String MQTT_PUBLISH_TOPIC;
extern String MQTT_SUBSCRIBE_TOPIC;
extern int SLEEP_TIME_MINUTES;

inline float average (float * array, int len)  // assuming array is int.
{
  long sum = 0L ;  // sum will be larger than an item, long for safety.
  for (int i = 0 ; i < len ; i++)
    sum += array [i] ;
  return  ((float) sum) / len ;  // average will be fractional, so float may be appropriate.
}

//most occuring
inline float mode(float array[100]) {

    float modeMap[100];
    
    int maxEl = array[0];
    int maxCount = 1;

    for (int i = 0; i < 100; i++) {
        int el = array[i];
        if (modeMap[el] == 0) {
            modeMap[el] = 1;
        }
        else {
            modeMap[el]++;
        }

        if (modeMap[el] > maxCount) {
            maxEl = el;
            maxCount = modeMap[el];
        }
    }
    return maxEl;
}

inline String loadConfig()
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
    key.trim();
    value.trim();

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
    else if (key == "SLEEP_TIME_MINUTES")
      SLEEP_TIME_MINUTES = value.toInt();
  }

  file.close();
  return "";
}