#include <Arduino.h>
#include <Wire.h>

#include "ens210.h"

ENS210 ens210;

void setup()
{
    Serial.begin(9600);
    Serial.println();

    Wire.begin();
    ens210.begin();
    ens210.init();

    if (ens210.isConnected() == false)
    {
        Serial.println("Error -- The ENS210 is not connected.");
        while(1);
    }

    Serial.print("Starting continous mode..");
    while (ens210.startContinuousMeasure() != RESULT_OK)
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
    ens210.wait();

    if (ens210.update() == RESULT_OK)
    {
        float temperatureCelsius = ens210.getTempCelsius();
        float humidityPercent    = ens210.getHumidityPercent();

        Serial.print("Temperature:");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity:");
        Serial.print(humidityPercent);
        Serial.println("%");
    }
}