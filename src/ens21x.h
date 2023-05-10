#ifndef SCIOSENSE_ENS21X_H
#define SCIOSENSE_ENS21X_H

#include <stdint.h>

#include <Arduino.h>
#include <Stream.h>
#include <Wire.h>

namespace ScioSense
{
    class ENS21x
    {
    public:

        // System Timing Characteristics
        struct SystemTiming
        {
            static constexpr uint16_t BOOTING                 = 2;      // Booting time in ms (also after reset, or going to high power)
            static constexpr uint16_t CONVERSION_SINGLE_SHOT  = 130;    // Conversion time in ms for single shot T/H measurement
            static constexpr uint16_t CONVERSION_CONTINUOUS   = 238;    // Conversion time in ms for continuous T/H measurement
        };

        // System configuration (SYS_CTRL)
        enum class SystemControl : uint8_t
        {
            DISABLE_LOW_POWER   = 0,
            ENABLE_LOW_POWER    = 1 << 0,
            RESET               = 1 << 7
        };

        // Addresses of the ENS21x registers
        enum class RegisterAddress : uint8_t
        {
            PART_ID     = 0x00,     // Identifies the part as ENS21x
            DIE_REV     = 0x02,     // Identifies the die revision version
            UID         = 0x04,     // Unique identifier
            SYS_CTRL    = 0x10,     // System configuration
            SYS_STAT    = 0x11,     // System status
            SENS_RUN    = 0x21,     // The run mode (single shot or continuous)
            SENS_START  = 0x22,     // Start measurement
            SENS_STOP   = 0x23,     // Stop continuous measurement
            SENS_STAT   = 0x24,     // Sensor status (idle or measuring)
            T_VAL       = 0x30,     // Temperature readout
            H_VAL       = 0x33      // Relative humidity readout
        };

        enum class Sensor: uint8_t
        {
            TEMPERATURE                 = 1 << 0,
            HUMIDITY                    = 1 << 1,
            TEMPERATURE_AND_HUMIDITY    = TEMPERATURE | HUMIDITY
        };

        enum class Result : uint32_t
        {
            STATUS_I2C_ERROR = 4, // There was an I2C communication error, `read`ing the value.
            STATUS_CRC_ERROR = 3, // The value was read, but the CRC over the payload (valid and data) does not match.
            STATUS_INVALID   = 2, // The value was read, the CRC matches, but the data is invalid (e.g. the measurement was not yet finished).
            STATUS_OK        = 1  // The value was read, the CRC matches, and data is valid.
        };

    public:
        ENS21x();
        virtual ~ENS21x();

    public:
        virtual bool begin(TwoWire& twoWire = Wire, uint8_t address= 0x43); // Connnects to ENS21x using the given TwoWire object and reads PART_ID, DIE_REV and UID; returns the result of isConnected()
        virtual bool isConnected() = 0;                                     // checks if the read PART_ID matches the expected value; returns true, if so.

    public:
        Result update(uint64_t ms = SystemTiming::CONVERSION_CONTINUOUS);
        Result singleShotMeasure        (Sensor sensor = Sensor::TEMPERATURE_AND_HUMIDITY);     // invokes a single shot mode measuring for the given sensor
        Result startContinuousMeasure   (Sensor sensor = Sensor::TEMPERATURE_AND_HUMIDITY);     // starts the continuous measure mode for the given sensor
        Result stopContinuousMeasure    (Sensor sensor = Sensor::TEMPERATURE_AND_HUMIDITY);     // stops the continuous measure mode for the given sensor
        Result setLowPower(bool enable);                                                        // en-/disables the low power mode
        Result reset();                                                                         // reset the device

    public:
        uint16_t getPartId();   // returns the PART_ID or zero if invalid
        uint16_t getDieRev();   // returns the DIE_REV or zero if invalid
        uint64_t getUid();      // returns the UID or zero if invalid

    public:
        void setSolderCorrection(int16_t correction = 50 * 64 / 1000);  // Sets the solder correction (default is 50mK) - used to calculate temperature and humidity values
        float getTempKelvin();                                          // Converts and returns measurement data in Kelvin
        float getTempCelsius();                                         // Converts and returns measurement data in Celsius
        float getTempFahrenheit();                                      // Converts and returns measurement data in Fahrenheit
        float getHumidityPercent();                                     // Converts and returns measurement data in %RH
        float getAbsoluteHumidityPercent();                             // Converts and returns measurement data in %aH

    public:
        uint16_t getDataT(); // returns raw temperature data; e.g. to be used as temperature compensation for ENS160 (TEMP_IN)
        uint16_t getDataH(); // returns raw humidity data; e.g. to be used as humidity compensation for ENS160 (RH_IN)
        Result getStatusT(); // returns the read status of the last read temperature data
        Result getStatusH(); // returns the read status of the last read humidity data

    public:
        void enableDebugging(Stream& debugStream);     // Enables the debug log. The output is written to the given debugStream
        void disableDebugging();                       // Stops the debug log if enabled. Does nothing otherwise.

    protected:
        Result read(RegisterAddress address, uint8_t* data, size_t size);
        Result write(RegisterAddress address, uint8_t* data, size_t size);
        template<class T> Result read(RegisterAddress address, T& data);
        template<class T> Result write(RegisterAddress address, T data);

    protected:
        void readIdentifiers();

    protected:
        const char* debugPrefix= "ENS21x debug -- ";
        const char* toString(Result& result);
        void debug(const char* msg);
        void debug(const char* msg, Result& result);
        void debug(const char* msg, uint8_t* data, size_t size, Result& result);
        template<class T> void debug(const char* msg, T data);

    protected:
        uint8_t slaveAddress;
        int16_t solderCorrection;

        uint16_t partId;
        uint16_t dieRev;
        uint64_t uid;

        uint16_t tData;
        uint16_t hData;
        Result tStatus;
        Result hStatus;

    private:
        uint32_t crc7(uint32_t val);
        Result checkData(uint32_t data);

    private:
        TwoWire* wire;
        Stream* debugStream;
    };
}

#endif //SCIOSENSE_ENS21X_H