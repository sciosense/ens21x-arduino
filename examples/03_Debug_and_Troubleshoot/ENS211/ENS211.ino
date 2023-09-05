#include <Arduino.h>
#include <Wire.h>

#include "ens211.h"

using namespace ScioSense;

ENS211 ens211;
ENS211::Result result;

void validateData(ENS211::Result result)
{
    switch (result)
    {
        case ENS211::Result::STATUS_I2C_ERROR: Serial.println("i2c-error");       break;
        case ENS211::Result::STATUS_CRC_ERROR: Serial.println("crc-error");       break;
        case ENS211::Result::STATUS_INVALID:   Serial.println("data-invalid");    break;
        case ENS211::Result::STATUS_OK:        Serial.println("ok");              break;
        default:                               Serial.println("unknown-status");  break;
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.println();

    ens211.enableDebugging(Serial);

    Wire.begin();
    ens211.begin();

    if (ens211.isConnected() == false)
    {
        Serial.println("Error -- The ENS211 is not connected.");
        while(1);
    }

    if (ens211.reset() != ENS211::Result::STATUS_OK)
    {
        Serial.println("Error -- Failed to reset the device.");
        while(1);
    }

    if (ens211.singleShotMeasure() != ENS211::Result::STATUS_OK)
    {
        Serial.println("Error -- Single shot measure failed.");

        Serial.print("Temperature status: ");
        validateData(ens211.getStatusT());

        Serial.print("Humidity status: ");
        validateData(ens211.getStatusH());

        while(1);
    }

    if (ens211.startContinuousMeasure() != ENS211::Result::STATUS_OK)
    {
        Serial.println("Error -- Starting continuous mode failed.");
        while(1);
    }

    if (ens211.update() != ENS211::Result::STATUS_OK)
    {
        Serial.println("Error -- Updating measurement data failed.");

        Serial.print("Temperature status: ");
        validateData(ens211.getStatusT());

        Serial.print("Humidity status: ");
        validateData(ens211.getStatusH());

        while(1);
    }

    if (ens211.stopContinuousMeasure() != ENS211::Result::STATUS_OK)
    {
        Serial.println("Error -- Stopping continuous mode failed.");
        while(1);
    }

    ens211.disableDebugging();

    Serial.println();
    Serial.println("Done. All OK!");

    Serial.println();
    Serial.println("----------------------------------------");
}

void loop()
{
    if (ens211.singleShotMeasure() == ENS211::Result::STATUS_OK)
    {
        float temperatureCelsius = ens211.getTempCelsius();
        float humidityPercent    = ens211.getHumidityPercent();

        Serial.print("Temperature: ");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity: ");
        Serial.print(humidityPercent);
        Serial.println("%");
        ens211.getStatusH();
    }
}