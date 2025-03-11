#include <Arduino.h>
#include <Wire.h>

#include "ens213a.h"

ENS213A ens213a;
Result result;

void validateData(Result result)
{
    switch (result)
    {
        case RESULT_IO_ERROR:       Serial.println("i2c-error");       break;
        case RESULT_CHECKSUM_ERROR: Serial.println("crc-error");       break;
        case RESULT_INVALID:        Serial.println("data-invalid");    break;
        case RESULT_OK:             Serial.println("ok");              break;
        default:                    Serial.println("unknown-status");  break;
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.println();

    ens213a.enableDebugging(Serial);

    Wire.begin();
    ens213a.begin();
    ens213a.init();

    if (ens213a.isConnected() == false)
    {
        Serial.println("Error -- The ENS213A is not connected.");
        while(1);
    }

    if (ens213a.reset() != RESULT_OK)
    {
        Serial.println("Error -- Failed to reset the device.");
        while(1);
    }

    result = ens213a.singleShotMeasure();
    if (result != RESULT_OK)
    {
        Serial.println("Error -- Single shot measure failed.");
        validateData(result);

        Serial.print("Temperature status: ");
        validateData(Ens21x_CheckData(ens213a.dataBuffer));

        Serial.print("Humidity status: ");
        validateData(Ens21x_CheckData(ens213a.dataBuffer + 3));

        while(1);
    }

    if (ens213a.startContinuousMeasure() != RESULT_OK)
    {
        Serial.println("Error -- Starting continuous mode failed.");
        while(1);
    }

    result = ens213a.update();
    if (result != RESULT_OK)
    {
        Serial.println("Error -- Updating measurement data failed.");
        validateData(result);

        Serial.print("Temperature status: ");
        validateData(Ens21x_CheckData(ens213a.dataBuffer));

        Serial.print("Humidity status: ");
        validateData(Ens21x_CheckData(ens213a.dataBuffer + 3));

        while(1);
    }

    if (ens213a.stopContinuousMeasure() != RESULT_OK)
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
    if (ens213a.singleShotMeasure() == RESULT_OK)
    {
        float temperatureCelsius = ens213a.getTempCelsius();
        float humidityPercent    = ens213a.getHumidityPercent();

        Serial.print("Temperature: ");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity: ");
        Serial.print(humidityPercent);
        Serial.println("%");
    }
}