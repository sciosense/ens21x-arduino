#include <Arduino.h>
#include <Wire.h>

#include "ens210.h"

using namespace ScioSense;

ENS210 ens210;

void setup()
{
    Serial.begin(9600);
    Serial.println();

    Wire.begin();
    ens210.begin();

    if (ens210.isConnected() == false)
    {
        Serial.println("Error -- The ENS210 is not connected.");
        while(1);
    }
}

void loop()
{
    if (ens210.singleShotMeasure() == ENS210::Result::STATUS_OK)
    {
        float temperatureCelsius = ens210.getTempCelsius();
        float humidityPercent    = ens210.getHumidityPercent();

        Serial.print("Temperature: ");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity: ");
        Serial.print(humidityPercent);
        Serial.println("%");
    }

    delay(1000);
}