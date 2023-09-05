#include <Arduino.h>
#include <Wire.h>

#include "ens215.h"

using namespace ScioSense;

ENS215 ens215;
ENS215::Result result;

void validateData(ENS215::Result result)
{
    switch (result)
    {
        case ENS215::Result::STATUS_I2C_ERROR: Serial.println("i2c-error");       break;
        case ENS215::Result::STATUS_CRC_ERROR: Serial.println("crc-error");       break;
        case ENS215::Result::STATUS_INVALID:   Serial.println("data-invalid");    break;
        case ENS215::Result::STATUS_OK:        Serial.println("ok");              break;
        default:                               Serial.println("unknown-status");  break;
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.println();

    ens215.enableDebugging(Serial);

    Wire.begin();
    ens215.begin();

    if (ens215.isConnected() == false)
    {
        Serial.println("Error -- The ENS215 is not connected.");
        while(1);
    }

    if (ens215.reset() != ENS215::Result::STATUS_OK)
    {
        Serial.println("Error -- Failed to reset the device.");
        while(1);
    }

    if (ens215.singleShotMeasure() != ENS215::Result::STATUS_OK)
    {
        Serial.println("Error -- Single shot measure failed.");

        Serial.print("Temperature status: ");
        validateData(ens215.getStatusT());

        Serial.print("Humidity status: ");
        validateData(ens215.getStatusH());

        while(1);
    }

    if (ens215.startContinuousMeasure() != ENS215::Result::STATUS_OK)
    {
        Serial.println("Error -- Starting continuous mode failed.");
        while(1);
    }

    if (ens215.update() != ENS215::Result::STATUS_OK)
    {
        Serial.println("Error -- Updating measurement data failed.");

        Serial.print("Temperature status: ");
        validateData(ens215.getStatusT());

        Serial.print("Humidity status: ");
        validateData(ens215.getStatusH());

        while(1);
    }

    if (ens215.stopContinuousMeasure() != ENS215::Result::STATUS_OK)
    {
        Serial.println("Error -- Stopping continuous mode failed.");
        while(1);
    }

    ens215.disableDebugging();

    Serial.println();
    Serial.println("Done. All OK!");

    Serial.println();
    Serial.println("----------------------------------------");
}

void loop()
{
    if (ens215.singleShotMeasure() == ENS215::Result::STATUS_OK)
    {
        float temperatureCelsius = ens215.getTempCelsius();
        float humidityPercent    = ens215.getHumidityPercent();

        Serial.print("Temperature: ");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity: ");
        Serial.print(humidityPercent);
        Serial.println("%");
        ens215.getStatusH();
    }
}