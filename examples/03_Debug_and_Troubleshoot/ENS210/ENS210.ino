#include <Arduino.h>
#include <Wire.h>

#include "ens210.h"

using namespace ScioSense;

ENS210 ens210;
ENS210::Result result;

void validateData(ENS210::Result result)
{
    switch (result)
    {
        case ENS210::Result::STATUS_I2C_ERROR: Serial.println("i2c-error");       break;
        case ENS210::Result::STATUS_CRC_ERROR: Serial.println("crc-error");       break;
        case ENS210::Result::STATUS_INVALID:   Serial.println("data-invalid");    break;
        case ENS210::Result::STATUS_OK:        Serial.println("ok");              break;
        default:                               Serial.println("unknown-status");  break;
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.println();

    ens210.enableDebugging(Serial);

    Wire.begin();
    ens210.begin();

    if (ens210.isConnected() == false)
    {
        Serial.println("Error -- The ENS210 is not connected.");
        while(1);
    }

    if (ens210.reset() != ENS210::Result::STATUS_OK)
    {
        Serial.println("Error -- Failed to reset the device.");
        while(1);
    }

    if (ens210.singleShotMeasure() != ENS210::Result::STATUS_OK)
    {
        Serial.println("Error -- Single shot measure failed.");

        Serial.print("Temperature status: ");
        validateData(ens210.getStatusT());

        Serial.print("Humidity status: ");
        validateData(ens210.getStatusH());

        while(1);
    }

    if (ens210.startContinuousMeasure() != ENS210::Result::STATUS_OK)
    {
        Serial.println("Error -- Starting continuous mode failed.");
        while(1);
    }

    if (ens210.update() != ENS210::Result::STATUS_OK)
    {
        Serial.println("Error -- Updating measurement data failed.");

        Serial.print("Temperature status: ");
        validateData(ens210.getStatusT());

        Serial.print("Humidity status: ");
        validateData(ens210.getStatusH());

        while(1);
    }

    if (ens210.stopContinuousMeasure() != ENS210::Result::STATUS_OK)
    {
        Serial.println("Error -- Stopping continuous mode failed.");
        while(1);
    }

    ens210.disableDebugging();

    Serial.println();
    Serial.println("Done. All OK!");

    Serial.println();
    Serial.println("----------------------------------------");
}

void loop()
{
    if (ens210.singleShotMeasure() == ENS210::Result::STATUS_OK)
    {
        float temperatureCelsius = ens210.getTempCelsius();
        float humidityPercent    = ens210.getHumidityPercent();

        Serial.print("Temperature: ");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity: ");
        Serial.print(humidityPercent);
        Serial.println("%");
        ens210.getStatusH();
    }
}