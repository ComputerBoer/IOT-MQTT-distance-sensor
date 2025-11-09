#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>


WiFiClient network;
MQTTClient mqtt = MQTTClient(256);

unsigned long lastPublishTime = 0;

void messageHandler(String &topic, String &payload) {
  Serial.println("ESP8266 - received from MQTT:");
  Serial.println("- topic: " + topic);
  Serial.println("- payload:");
  Serial.println(payload);
}

bool connectToMQTT(const char* Broker_Address, int Port, const char* Client_id,const char* Username, const char* Password, String Subscribe_topic = "") {
  // Connect to the MQTT broker
  mqtt.begin(Broker_Address, Port, network);

  // Create a handler for incoming messages
  mqtt.onMessage(messageHandler);

  Serial.print("ESP8266 - Connecting to MQTT broker");

  while (!mqtt.connect(Client_id, Username, Password)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  if (!mqtt.connected()) {
    Serial.println("ESP8266 - MQTT broker Timeout!");
    return false;
  }

  // Subscribe to a topic, the incoming messages are processed by messageHandler() function
  if(Subscribe_topic != ""){
  if (mqtt.subscribe(Subscribe_topic))
    Serial.print("ESP8266 - Subscribed to the topic: ");
  else
    Serial.print("ESP8266 - Failed to subscribe to the topic: ");
  Serial.println(Subscribe_topic);
  }

  Serial.println("ESP8266 - MQTT broker Connected!");
  return true;
}

void sendToMQTT(String Publish_Topic, float measurement) {
  JsonDocument message;
  message["timestamp"] = millis();
  message["measurement"] = measurement;  // Or you can read data from other sensors

  char messageBuffer[512];
  serializeJson(message, messageBuffer);

  mqtt.publish(Publish_Topic, messageBuffer);

  Serial.println("ESP8266 - sent to MQTT:");
  Serial.print("- topic: ");
  Serial.println(Publish_Topic);
  Serial.print("- payload:");
  Serial.println(messageBuffer);
}

