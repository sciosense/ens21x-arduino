#include <Arduino.h>
#include <Wire.h>

#include "ens212.h"

using namespace ScioSense;

ENS212 ens212;
ENS212::Result result;

void validateData(ENS212::Result result)
{
    switch (result)
    {
        case ENS212::Result::STATUS_I2C_ERROR: Serial.println("i2c-error");       break;
        case ENS212::Result::STATUS_CRC_ERROR: Serial.println("crc-error");       break;
        case ENS212::Result::STATUS_INVALID:   Serial.println("data-invalid");    break;
        case ENS212::Result::STATUS_OK:        Serial.println("ok");              break;
        default:                               Serial.println("unknown-status");  break;
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.println();

    ens212.enableDebugging(Serial);

    Wire.begin();
    ens212.begin();

    if (ens212.isConnected() == false)
    {
        Serial.println("Error -- The ENS212 is not connected.");
        while(1);
    }

    if (ens212.reset() != ENS212::Result::STATUS_OK)
    {
        Serial.println("Error -- Failed to reset the device.");
        while(1);
    }

    if (ens212.singleShotMeasure() != ENS212::Result::STATUS_OK)
    {
        Serial.println("Error -- Single shot measure failed.");

        Serial.print("Temperature status: ");
        validateData(ens212.getStatusT());

        Serial.print("Humidity status: ");
        validateData(ens212.getStatusH());

        while(1);
    }

    if (ens212.startContinuousMeasure() != ENS212::Result::STATUS_OK)
    {
        Serial.println("Error -- Starting continuous mode failed.");
        while(1);
    }

    if (ens212.update() != ENS212::Result::STATUS_OK)
    {
        Serial.println("Error -- Updating measurement data failed.");

        Serial.print("Temperature status: ");
        validateData(ens212.getStatusT());

        Serial.print("Humidity status: ");
        validateData(ens212.getStatusH());

        while(1);
    }

    if (ens212.stopContinuousMeasure() != ENS212::Result::STATUS_OK)
    {
        Serial.println("Error -- Stopping continuous mode failed.");
        while(1);
    }

    ens212.disableDebugging();

    Serial.println();
    Serial.println("Done. All OK!");

    Serial.println();
    Serial.println("----------------------------------------");
}

void loop()
{
    if (ens212.singleShotMeasure() == ENS212::Result::STATUS_OK)
    {
        float temperatureCelsius = ens212.getTempCelsius();
        float humidityPercent    = ens212.getHumidityPercent();

        Serial.print("Temperature: ");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity: ");
        Serial.print(humidityPercent);
        Serial.println("%");
        ens212.getStatusH();
    }
}