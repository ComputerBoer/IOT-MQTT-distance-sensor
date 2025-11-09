#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <iostream>
#include <functional>
#include <future>

const int ledPin = 2;
const int trigPin = 12;
const int echoPin = 14;

// define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

float getDistanceCm()
{
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    long duration = pulseIn(echoPin, HIGH);

    // Calculate the distance
    float distance = duration * SOUND_VELOCITY / 2;

    return distance > 400 ? 400 : round(distance);
}
void blink(int duration = 500)
{
    digitalWrite(ledPin, LOW);
    delay(duration);
    digitalWrite(ledPin, HIGH);
    delay(duration);
}
bool wifiConnect(String networkname, String password)
{
    byte i = 0;
    WiFi.begin(networkname, password);
    Serial.print("Connecting");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        i++;
        if(i > 30) {
            Serial.println("Wifi connection failed");
            return false;
        }
    }
    Serial.println();
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    return true;
}
