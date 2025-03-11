#include <Arduino.h>
#include <Wire.h>

#include "ens211.h"

ENS211 ens211;

void setup()
{
    Serial.begin(9600);
    Serial.println();

    Wire.begin();
    ens211.begin();
    ens211.init();

    if (ens211.isConnected() == false)
    {
        Serial.println("Error -- The ENS211 is not connected.");
        while(1);
    }

    Serial.print("Starting continous mode..");
    while (ens211.startContinuousMeasure() != RESULT_OK)
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
    ens211.wait();

    if (ens211.update() == RESULT_OK)
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
}