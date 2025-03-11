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

    Serial.print("Starting continous mode..");
    while (ens213a.startContinuousMeasure() != RESULT_OK)
    {
        Serial.print(".");
        delay(ENS21X_SYSTEM_TIMING_BOOTING);
    }
    Serial.println(" Done!");

    Serial.println();
    Serial.println("----------------------------------------");
}

void loop()
{
    ens213a.wait();

    if (ens213a.update() == RESULT_OK)
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
}