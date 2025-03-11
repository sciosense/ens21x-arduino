#include <Arduino.h>
#include <Wire.h>

#include "ens215.h"

ENS215 ens215;

void setup()
{
    Serial.begin(9600);
    Serial.println();

    Wire.begin();
    ens215.begin();
    ens215.init();

    if (ens215.isConnected() == false)
    {
        Serial.println("Error -- The ENS215 is not connected.");
        while(1);
    }

    Serial.print("Starting continous mode..");
    while (ens215.startContinuousMeasure() != RESULT_OK)
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
    ens215.wait();

    if (ens215.update() == RESULT_OK)
    {
        float temperatureCelsius = ens215.getTempCelsius();
        float humidityPercent    = ens215.getHumidityPercent();

        Serial.print("Temperature:");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity:");
        Serial.print(humidityPercent);
        Serial.println("%");
    }
}