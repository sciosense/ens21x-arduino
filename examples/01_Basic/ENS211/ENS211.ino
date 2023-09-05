#include <Arduino.h>
#include <Wire.h>

#include "ens211.h"

using namespace ScioSense;

ENS211 ens211;

void setup()
{
    Serial.begin(9600);
    Serial.println();

    Wire.begin();
    ens211.begin();

    if (ens211.isConnected() == false)
    {
        Serial.println("Error -- The ENS211 is not connected.");
        while(1);
    }
}

void loop()
{
    if (ens211.singleShotMeasure() == ENS211::Result::STATUS_OK)
    {
        float temperatureCelsius = ens211.getTempCelsius();
        float humidityPercent    = ens211.getHumidityPercent();

        Serial.print("Temperature:");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity:");
        Serial.print(humidityPercent);
        Serial.println("%");
    }

    delay(1000);
}