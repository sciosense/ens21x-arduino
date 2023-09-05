#include <Arduino.h>
#include <Wire.h>

#include "ens213a.h"

using namespace ScioSense;

ENS213A ens213a;

void setup()
{
    Serial.begin(9600);
    Serial.println();

    Wire.begin();
    ens213a.begin();

    if (ens213a.isConnected() == false)
    {
        Serial.println("Error -- The ENS213a is not connected.");
        while(1);
    }

    ens213a.reset();

    Serial.print("Starting continous mode..");
    while (ens213a.startContinuousMeasure() != ENS213A::Result::STATUS_OK)
    {
        Serial.print(".");
        delay(ENS213A::SystemTiming::BOOTING);
    }
    Serial.println(" Done!");

    Serial.println();
    Serial.println("----------------------------------------");
}

void loop()
{
    if (ens213a.update() == ENS213A::Result::STATUS_OK)
    {
        float temperatureCelsius = ens213a.getTempCelsius();
        float humidityPercent    = ens213a.getHumidityPercent();

        Serial.print("Temperature:");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity:");
        Serial.print(humidityPercent);
        Serial.println("%");
        ens213a.getStatusH();
    }
}