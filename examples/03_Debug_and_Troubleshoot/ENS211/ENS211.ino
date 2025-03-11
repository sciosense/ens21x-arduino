#include <Arduino.h>
#include <Wire.h>

#include "ens211.h"

ENS211 ens211;
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

    ens211.enableDebugging(Serial);

    Wire.begin();
    ens211.begin();
    ens211.init();

    if (ens211.isConnected() == false)
    {
        Serial.println("Error -- The ENS211 is not connected.");
        while(1);
    }

    if (ens211.reset() != RESULT_OK)
    {
        Serial.println("Error -- Failed to reset the device.");
        while(1);
    }

    result = ens211.singleShotMeasure();
    if (result != RESULT_OK)
    {
        Serial.println("Error -- Single shot measure failed.");
        validateData(result);

        Serial.print("Temperature status: ");
        validateData(Ens21x_CheckData(ens211.dataBuffer));

        Serial.print("Humidity status: ");
        validateData(Ens21x_CheckData(ens211.dataBuffer + 3));

        while(1);
    }

    if (ens211.startContinuousMeasure() != RESULT_OK)
    {
        Serial.println("Error -- Starting continuous mode failed.");
        while(1);
    }

    result = ens211.update();
    if (result != RESULT_OK)
    {
        Serial.println("Error -- Updating measurement data failed.");
        validateData(result);

        Serial.print("Temperature status: ");
        validateData(Ens21x_CheckData(ens211.dataBuffer));

        Serial.print("Humidity status: ");
        validateData(Ens21x_CheckData(ens211.dataBuffer + 3));

        while(1);
    }

    if (ens211.stopContinuousMeasure() != RESULT_OK)
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
    if (ens211.singleShotMeasure() == RESULT_OK)
    {
        float temperatureCelsius = ens211.getTempCelsius();
        float humidityPercent    = ens211.getHumidityPercent();

        Serial.print("Temperature: ");
        Serial.print(temperatureCelsius);
        Serial.print("°C\t");

        Serial.print("Humidity: ");
        Serial.print(humidityPercent);
        Serial.println("%");
    }
}