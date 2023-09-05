#include <Arduino.h>
#include <Wire.h>

#include "ens215.h"

using namespace ScioSense;

ENS215 ens215;

void setup()
{
    Serial.begin(9600);
    Serial.println();

    Wire.begin();
    ens215.begin();

    if (ens215.isConnected() == false)
    {
        Serial.println("Error -- The ENS215 is not connected.");
        while(1);
    }

    ens215.reset();

    Serial.print("Starting continous mode..");
    while (ens215.startContinuousMeasure() != ENS215::Result::STATUS_OK)
    {
        Serial.print(".");
        delay(ENS215::SystemTiming::BOOTING);
    }
    Serial.println(" Done!");

    Serial.println();
    Serial.println("----------------------------------------");
}

void loop()
{
    if (ens215.update() == ENS215::Result::STATUS_OK)
    {
        float temperatureCelsius = ens215.getTempCelsius();
        float humidityPercent    = ens215.getHumidityPercent();

        Serial.print("Temperature:");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity:");
        Serial.print(humidityPercent);
        Serial.println("%");
        ens215.getStatusH();
    }
}