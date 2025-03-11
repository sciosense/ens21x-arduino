#include <Arduino.h>
#include <Wire.h>

#include "ens212.h"

ENS212 ens212;

void setup()
{
    Serial.begin(9600);
    Serial.println();

    Wire.begin();
    ens212.begin();
    ens212.init();

    if (ens212.isConnected() == false)
    {
        Serial.println("Error -- The ENS212 is not connected.");
        while(1);
    }

    Serial.print("Starting continous mode..");
    while (ens212.startContinuousMeasure() != RESULT_OK)
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
    ens212.wait();

    if (ens212.update() == RESULT_OK)
    {
        float temperatureCelsius = ens212.getTempCelsius();
        float humidityPercent    = ens212.getHumidityPercent();

        Serial.print("Temperature:");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity:");
        Serial.print(humidityPercent);
        Serial.println("%");
    }
}