#ifndef SCIOSENSE_ENS21X_H
#define SCIOSENSE_ENS21X_H

#include <stdint.h>

#include <Arduino.h>
#include <Stream.h>
#include <Wire.h>

#include "lib/ens21x/ScioSense_Ens21x.h"
#include "lib/io/ScioSense_IOInterface_Arduino_I2C.h"


class ENS21x : public ScioSense_Ens21x
{
public:
    ENS21x();
    virtual ~ENS21x();

public:
    virtual void begin(TwoWire* twoWire, uint8_t address = 0x43);       // Connnects to ENS21x using the given TwoWire object and reads PART_ID, DIE_REV and UID; returns the result of isConnected()
    virtual bool isConnected();                                         // checks if the read PART_ID matches the expected value; returns true, if so.
    inline bool init();                                                 // resets the device and reads IDs

public:
    inline Result update                   (ENS21x_SensorConfiguration sensor = ENS21X_SENSOR_CONFIGURATION_TEMPERATURE_AND_HUMIDITY);
    inline Result singleShotMeasure        (ENS21x_SensorConfiguration sensor = ENS21X_SENSOR_CONFIGURATION_TEMPERATURE_AND_HUMIDITY);     // invokes a single shot mode measuring for the given sensor
    inline Result startContinuousMeasure   (ENS21x_SensorConfiguration sensor = ENS21X_SENSOR_CONFIGURATION_TEMPERATURE_AND_HUMIDITY);     // starts the continuous measure mode for the given sensor
    inline Result stopContinuousMeasure    (ENS21x_SensorConfiguration sensor = ENS21X_SENSOR_CONFIGURATION_TEMPERATURE_AND_HUMIDITY);     // stops the continuous measure mode for the given sensor
    inline Result setLowPower(bool enable);     // en-/disables the low power mode
    inline Result reset();                      // reset the device
    inline void   wait();                       // Waits for continuous measure to finish
public:
    inline uint16_t getPartId();   // returns the PART_ID or zero if invalid
    inline uint16_t getDieRev();   // returns the DIE_REV or zero if invalid
    inline uint64_t getUid();      // returns the UID or zero if invalid

public:
    inline void setSolderCorrection(int16_t correction = 50 * 64 / 1000);   // Sets the solder correction (default is 50mK) - used to calculate temperature and humidity values
    inline float getTempKelvin();                                           // Converts and returns measurement data in Kelvin
    inline float getTempCelsius();                                          // Converts and returns measurement data in Celsius
    inline float getTempFahrenheit();                                       // Converts and returns measurement data in Fahrenheit
    inline float getHumidityPercent();                                      // Converts and returns measurement data in %RH
    inline float getAbsoluteHumidityPercent();                              // Converts and returns measurement data in %aH

public:
    inline uint16_t getDataT(); // returns raw temperature data; e.g. to be used as temperature compensation for ENS160 (TEMP_IN)
    inline uint16_t getDataH(); // returns raw humidity data; e.g. to be used as humidity compensation for ENS160 (RH_IN)

public:
    inline void enableDebugging(Stream& debugStream);   // Enables the debug log. The output is written to the given debugStream
    inline void disableDebugging();                     // Stops the debug log if enabled. Does nothing otherwise.

protected:
    const char* debugPrefix= "ENS21x debug -- ";
    const char* toString(Result& result);
    inline void debug(const char* msg);
    inline void debug(const char* msg, Result& result);
    inline void debug(const char* msg, uint8_t* data, size_t size, Result& result);
    template<class T> void debug(const char* msg, T data);

protected:
    ScioSense_Arduino_I2c_Config        i2cConfig;

private:
    Stream* debugStream;
};

#include "ens21x.inl.h"
#endif //SCIOSENSE_ENS21X_H