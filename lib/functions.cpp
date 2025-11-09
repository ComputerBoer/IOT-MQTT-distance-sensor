#include "../lib/helpers.cpp"

int listlength = 10;
float *list;
byte i = 0;
byte cnt = 0;

float measure()
{
    float m = 0;
    while (cnt < 30)
    {
        float distanceCm = getDistanceCm();
        list[i] = distanceCm;

        delay(50);

        cnt++;
        i++;
        if (i >= listlength)
            i = 0;

        m = average(list, listlength);
        if (i % 5 == 0)
        {
            blink(100);
            Serial.println("Distance " + String(m) + "cm ");
        }
    }
    Serial.println("Returned distance " + String(m) + "cm");
    cnt = 0;
    
    return m;
}