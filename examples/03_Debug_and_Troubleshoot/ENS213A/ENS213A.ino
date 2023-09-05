#include <Arduino.h>
#include <Wire.h>

#include "ens213a.h"

using namespace ScioSense;

ENS213A ens213a;
ENS213A::Result result;

void validateData(ENS213A::Result result)
{
    switch (result)
    {
        case ENS213A::Result::STATUS_I2C_ERROR: Serial.println("i2c-error");       break;
        case ENS213A::Result::STATUS_CRC_ERROR: Serial.println("crc-error");       break;
        case ENS213A::Result::STATUS_INVALID:   Serial.println("data-invalid");    break;
        case ENS213A::Result::STATUS_OK:        Serial.println("ok");              break;
        default:                               Serial.println("unknown-status");  break;
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.println();

    ens213a.enableDebugging(Serial);

    Wire.begin();
    ens213a.begin();

    if (ens213a.isConnected() == false)
    {
        Serial.println("Error -- The ENS213A is not connected.");
        while(1);
    }

    if (ens213a.reset() != ENS213A::Result::STATUS_OK)
    {
        Serial.println("Error -- Failed to reset the device.");
        while(1);
    }

    if (ens213a.singleShotMeasure() != ENS213A::Result::STATUS_OK)
    {
        Serial.println("Error -- Single shot measure failed.");

        Serial.print("Temperature status: ");
        validateData(ens213a.getStatusT());

        Serial.print("Humidity status: ");
        validateData(ens213a.getStatusH());

        while(1);
    }

    if (ens213a.startContinuousMeasure() != ENS213A::Result::STATUS_OK)
    {
        Serial.println("Error -- Starting continuous mode failed.");
        while(1);
    }

    if (ens213a.update() != ENS213A::Result::STATUS_OK)
    {
        Serial.println("Error -- Updating measurement data failed.");

        Serial.print("Temperature status: ");
        validateData(ens213a.getStatusT());

        Serial.print("Humidity status: ");
        validateData(ens213a.getStatusH());

        while(1);
    }

    if (ens213a.stopContinuousMeasure() != ENS213A::Result::STATUS_OK)
    {
        Serial.println("Error -- Stopping continuous mode failed.");
        while(1);
    }

    ens213a.disableDebugging();

    Serial.println();
    Serial.println("Done. All OK!");

    Serial.println();
    Serial.println("----------------------------------------");
}

void loop()
{
    if (ens213a.singleShotMeasure() == ENS213A::Result::STATUS_OK)
    {
        float temperatureCelsius = ens213a.getTempCelsius();
        float humidityPercent    = ens213a.getHumidityPercent();

        Serial.print("Temperature: ");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity: ");
        Serial.print(humidityPercent);
        Serial.println("%");
        ens213a.getStatusH();
    }
}