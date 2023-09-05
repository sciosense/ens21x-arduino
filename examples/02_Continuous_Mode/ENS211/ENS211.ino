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

    ens211.reset();

    Serial.print("Starting continous mode..");
    while (ens211.startContinuousMeasure() != ENS211::Result::STATUS_OK)
    {
        Serial.print(".");
        delay(ENS211::SystemTiming::BOOTING);
    }
    Serial.println(" Done!");

    Serial.println();
    Serial.println("----------------------------------------");
}

void loop()
{
    if (ens211.update() == ENS211::Result::STATUS_OK)
    {
        float temperatureCelsius = ens211.getTempCelsius();
        float humidityPercent    = ens211.getHumidityPercent();

        Serial.print("Temperature:");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity:");
        Serial.print(humidityPercent);
        Serial.println("%");
        ens211.getStatusH();
    }
}