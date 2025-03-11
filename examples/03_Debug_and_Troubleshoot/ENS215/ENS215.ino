#include <Arduino.h>
#include <Wire.h>

#include "ens215.h"

ENS215 ens215;
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

    ens215.enableDebugging(Serial);

    Wire.begin();
    ens215.begin();
    ens215.init();

    if (ens215.isConnected() == false)
    {
        Serial.println("Error -- The ENS215 is not connected.");
        while(1);
    }

    if (ens215.reset() != RESULT_OK)
    {
        Serial.println("Error -- Failed to reset the device.");
        while(1);
    }

    result = ens215.singleShotMeasure();
    if (result != RESULT_OK)
    {
        Serial.println("Error -- Single shot measure failed.");
        validateData(result);

        Serial.print("Temperature status: ");
        validateData(Ens21x_CheckData(ens215.dataBuffer));

        Serial.print("Humidity status: ");
        validateData(Ens21x_CheckData(ens215.dataBuffer + 3));

        while(1);
    }

    if (ens215.startContinuousMeasure() != RESULT_OK)
    {
        Serial.println("Error -- Starting continuous mode failed.");
        while(1);
    }

    result = ens215.update();
    if (result != RESULT_OK)
    {
        Serial.println("Error -- Updating measurement data failed.");
        validateData(result);

        Serial.print("Temperature status: ");
        validateData(Ens21x_CheckData(ens215.dataBuffer));

        Serial.print("Humidity status: ");
        validateData(Ens21x_CheckData(ens215.dataBuffer + 3));

        while(1);
    }

    if (ens215.stopContinuousMeasure() != RESULT_OK)
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
    if (ens215.singleShotMeasure() == RESULT_OK)
    {
        float temperatureCelsius = ens215.getTempCelsius();
        float humidityPercent    = ens215.getHumidityPercent();

        Serial.print("Temperature: ");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity: ");
        Serial.print(humidityPercent);
        Serial.println("%");
    }
}