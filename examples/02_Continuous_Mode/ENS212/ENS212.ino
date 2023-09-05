#include <Arduino.h>
#include <Wire.h>

#include "ens212.h"

using namespace ScioSense;

ENS212 ens212;

void setup()
{
    Serial.begin(9600);
    Serial.println();

    Wire.begin();
    ens212.begin();

    if (ens212.isConnected() == false)
    {
        Serial.println("Error -- The ENS212 is not connected.");
        while(1);
    }

    ens212.reset();

    Serial.print("Starting continous mode..");
    while (ens212.startContinuousMeasure() != ENS212::Result::STATUS_OK)
    {
        Serial.print(".");
        delay(ENS212::SystemTiming::BOOTING);
    }
    Serial.println(" Done!");

    Serial.println();
    Serial.println("----------------------------------------");
}

void loop()
{
    if (ens212.update() == ENS212::Result::STATUS_OK)
    {
        float temperatureCelsius = ens212.getTempCelsius();
        float humidityPercent    = ens212.getHumidityPercent();

        Serial.print("Temperature:");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity:");
        Serial.print(humidityPercent);
        Serial.println("%");
        ens212.getStatusH();
    }
}