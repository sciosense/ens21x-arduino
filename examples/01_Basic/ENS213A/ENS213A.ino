#include <Arduino.h>
#include <Wire.h>

#include "ens213a.h"

ENS213A ens213a;

void setup()
{
    Serial.begin(9600);
    Serial.println();

    Wire.begin();
    ens213a.begin();
    ens213a.init();

    if (ens213a.isConnected() == false)
    {
        Serial.println("Error -- The ENS213A is not connected.");
        while(1);
    }
}

void loop()
{
    if (ens213a.singleShotMeasure() == RESULT_OK)
    {
        float temperatureCelsius = ens213a.getTempCelsius();
        float humidityPercent    = ens213a.getHumidityPercent();

        Serial.print("Temperature:");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity:");
        Serial.print(humidityPercent);
        Serial.println("%");
    }

    delay(1000);
}