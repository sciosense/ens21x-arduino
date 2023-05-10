#include <Arduino.h>
#include <Wire.h>

#include "ens21x.h"
#include "utils.h"

using namespace ScioSense::Utils;

namespace ScioSense
{
    ENS21x::~ENS21x() { }

    ENS21x::ENS21x()
    {
        wire                = nullptr;
        debugStream         = nullptr;
        solderCorrection    = 0;
        slaveAddress        = 0x43;

        partId              = 0;
        dieRev              = 0;
        uid                 = 0;
        tData               = 0;
        hData               = 0;
    }

    bool ENS21x::begin(TwoWire& twoWire, uint8_t address)
    {
        wire        = &twoWire;
        slaveAddress= address;
        readIdentifiers();

        return isConnected();
    }

    void ENS21x::enableDebugging(Stream& debugStream)
    {
        this->debugStream = &debugStream;
    }

    void ENS21x::disableDebugging()
    {
        debugStream = nullptr;
    }

    ENS21x::Result ENS21x::update(uint64_t ms)
    {
        delay(ms);

        uint8_t buffer[6];
        Result result = read(RegisterAddress::T_VAL, buffer, 6);
        if (result == Result::STATUS_OK)
        {
            tData   = endian::littleTo<uint16_t>(buffer);
            tStatus = checkData(endian::littleTo<uint32_t>(buffer));

            hData   = endian::littleTo<uint16_t>(buffer + 3);
            hStatus = checkData(endian::littleTo<uint32_t>(buffer + 3));
        }

        debug(__func__, result);
        return result;
    }

    ENS21x::Result ENS21x::singleShotMeasure(Sensor sensor)
    {
        Result result = write(RegisterAddress::SENS_START, sensor);
        if (result == Result::STATUS_OK)
        {
            result = update(SystemTiming::CONVERSION_SINGLE_SHOT);
            if (result == Result::STATUS_OK)
            {
                switch (sensor)
                {
                    case Sensor::TEMPERATURE:   result = tStatus; break;
                    case Sensor::HUMIDITY:      result = hStatus; break;
                    case Sensor::TEMPERATURE_AND_HUMIDITY:
                    {
                        result = tStatus != Result::STATUS_OK ? tStatus : hStatus;
                        break;
                    }
                }
            }
        }

        debug(__func__, result);
        return result;
    }

    ENS21x::Result ENS21x::startContinuousMeasure(Sensor sensor)
    {
        Result result = write(RegisterAddress::SENS_RUN, sensor);
        if (result == Result::STATUS_OK)
        {
            result = singleShotMeasure(sensor);
        }

        debug(__func__, result);
        return result;
    }

    ENS21x::Result ENS21x::stopContinuousMeasure(Sensor sensor)
    {
        Result result = write(RegisterAddress::SENS_STOP, sensor);

        debug(__func__, result);
        return result;
    }

    ENS21x::Result ENS21x::setLowPower(bool enable)
    {
        Result result;

        if (enable)
        {
            result = write(RegisterAddress::SYS_CTRL, SystemControl::ENABLE_LOW_POWER);
        }
        else
        {
            result = write(RegisterAddress::SYS_CTRL, SystemControl::DISABLE_LOW_POWER);
        }

        return result;
    }

    ENS21x::Result ENS21x::reset()
    {
        Result result = write(RegisterAddress::SYS_CTRL, SystemControl::RESET);
        if (result == Result::STATUS_OK)
        {
            delay(SystemTiming::BOOTING);
        }

        debug(__func__, result);
        return result;
    }

    uint16_t ENS21x::getPartId()
    {
        return partId;
    }

    uint16_t ENS21x::getDieRev()
    {
        return dieRev;
    }

    uint64_t ENS21x::getUid()
    {
        return uid;
    }

    void ENS21x::setSolderCorrection(int16_t correction)
    {
        solderCorrection = correction;
    }

    float ENS21x::getTempKelvin()
    {
        return (tData - solderCorrection) / 64.f;
    }

    float ENS21x::getTempCelsius()
    {
        return getTempKelvin() - (27315L / 100);
    }

    float ENS21x::getTempFahrenheit()
    {
        // Return m*F. This equals m*(1.8*(K-273.15)+32) = m*(1.8*K-273.15*1.8+32) = 1.8*m*K-459.67*m = 9*m*K/5 - 45967*m/100 = 9*m*t/320 - 45967*m/100
        // Note m is the multiplier, F is temperature in Fahrenheit, K is temperature in Kelvin, t is raw t_data value.
        // Uses F=1.8*(K-273.15)+32 and K=t/64.
        // The first multiplication stays below 32 bits (t:16, multiplier:11, 9:4)
        // The second multiplication stays below 32 bits (multiplier:10, 45967:16)

        return (9.f * (tData - solderCorrection) / 320.f) - 459.67f;
    }

    float ENS21x::getHumidityPercent()
    {
        // Return m*H. This equals m*(h/512) = (m*h)/512
        // Note m is the multiplier, H is the relative humidity in %RH, h is raw h_data value.
        // Uses H=h/512.

        return hData / 512.f;
    }

    float ENS21x::getAbsoluteHumidityPercent()//TODO cleanup
    {
        // taken from https://carnotcycle.wordpress.com/2012/08/04/how-to-convert-relative-humidity-to-absolute-humidity/
        // precision is about 0.1°C in range -30 to 35°C
        // August-Roche-Magnus   6.1094 exp(17.625 x T)/(T + 243.04)
        // Buck (1981)     6.1121 exp(17.502 x T)/(T + 240.97)
        // reference https://www.eas.ualberta.ca/jdwilson/EAS372_13/Vomel_CIRES_satvpformulae.html    // Use Buck (1981)

        static const float MOLAR_MASS_OF_WATER      = 18.01534;
        static const float UNIVERSAL_GAS_CONSTANT   = 8.21447215;
        return (6.1121 * pow(2.718281828,(17.67* this->getTempCelsius())/(this->getTempCelsius() + 243.5))* this->getHumidityPercent() *MOLAR_MASS_OF_WATER)/((273.15+ this->getTempCelsius() )*UNIVERSAL_GAS_CONSTANT);
    }

    uint16_t ENS21x::getDataT()
    {
        return tData;
    }

    uint16_t ENS21x::getDataH()
    {
        return hData;
    }

    ENS21x::Result ENS21x::getStatusT()
    {
        return tStatus;
    }

    ENS21x::Result ENS21x::getStatusH()
    {
        return hStatus;
    }

    ENS21x::Result ENS21x::read(RegisterAddress address, uint8_t* data, size_t size)
    {
        Result result = Result::STATUS_I2C_ERROR;

        wire->beginTransmission(slaveAddress);
        wire->write((uint8_t)address);
        if (wire->endTransmission() == 0) // 0 == success
        {
            wire->requestFrom(slaveAddress, size);
            if (wire->readBytes(data, size) == size)
            {
                result = Result::STATUS_OK;
            }
        }

        debug(__func__, data, size, result);
        return result;
    }

    ENS21x::Result ENS21x::write(RegisterAddress address, uint8_t* data, size_t size)
    {
        Result result = Result::STATUS_I2C_ERROR;

        wire->beginTransmission(slaveAddress);
        wire->write((uint8_t)address);
        wire->write(data, size);
        if (wire->endTransmission() == 0) // 0 == success
        {
            result = Result::STATUS_OK;
        }

        debug(__func__, data, size, result);
        return result;
    }

    template<class T>
    ENS21x::Result ENS21x::read(RegisterAddress address, T& data)
    {
        return read(address, (uint8_t*)&data, sizeof(data));
    }

    template<class T>
    ENS21x::Result ENS21x::write(RegisterAddress address, T data)
    {
        return write(address, (uint8_t*)&data, sizeof(data));
    }

    void ENS21x::readIdentifiers()
    {
        debug(__func__);

        setLowPower(false);
        delay(SystemTiming::BOOTING);

        read(RegisterAddress::PART_ID, partId);
        debug("PART_ID: ", partId);

        read(RegisterAddress::DIE_REV, dieRev);
        debug("DIE_REV: ", dieRev);

        read(RegisterAddress::UID, uid);
        debug("UID:     ", uid);

        setLowPower(true);
    }

    void ENS21x::debug(const char* msg)
    {
        if (debugStream)
        {
            debugStream->print(debugPrefix);
            debugStream->println(msg);
        }
    }

    void ENS21x::debug(const char* msg, Result& result)
    {
        debug(msg, nullptr, 0, result);
    }

    void ENS21x::debug(const char* msg, uint8_t* data, size_t size, Result& result)
    {
        if (debugStream)
        {
            debugStream->print(debugPrefix);
            debugStream->print(msg);

            for (size_t i = 0; i < size; i++)
            {
                debugStream->print(" 0x");
                debugStream->print(data[i], HEX);
            }

            debugStream->print(" status: ");
            debugStream->println(toString(result));
        }
    }

    template<class T>
    void ENS21x::debug(const char* msg, T data)
    {
        if (debugStream)
        {
            debugStream->print(debugPrefix);
            debugStream->print(msg);
            debugStream->print(" 0x");
            debugStream->print((uint32_t)data, HEX);
            debugStream->println();
        }
    }

    const char* ENS21x::toString(Result& result)
    {
        switch (result)
        {
            case Result::STATUS_I2C_ERROR: return "i2c-error";
            case Result::STATUS_CRC_ERROR: return "crc-error";
            case Result::STATUS_INVALID:   return "data-invalid";
            case Result::STATUS_OK:        return "ok";
            default:                       return "unknown-status";
        }
    }

    // Compute the CRC-7 of 'val' (should only have 17 bits)
    // https://en.wikipedia.org/wiki/Cyclic_redundancy_check#Computation
    uint32_t ENS21x::crc7(uint32_t val)
    {
        static const uint8_t CRC7WIDTH  = 7;                            // A 7 bits CRC has polynomial of 7th order, which has 8 terms
        static const uint8_t CRC7POLY   = 0x89;                         // The 8 coefficients of the polynomial
        static const uint8_t CRC7IVEC   = 0x7F;                         // Initial vector has all 7 bits high
        static const uint8_t DATA7WIDTH = 17;
        static const uint32_t DATA7MASK = ((1UL << DATA7WIDTH) - 1);    // 0b 0 1111 1111 1111 1111
        static const uint32_t DATA7MSB  = (1UL << (DATA7WIDTH - 1));    // 0b 1 0000 0000 0000 0000

        // Setup polynomial
        uint32_t pol = CRC7POLY;

        // Align polynomial with data
        pol = pol << (DATA7WIDTH - CRC7WIDTH - 1);

        // Loop variable (indicates which bit to test, start with highest)
        uint32_t bit = DATA7MSB;

        // Make room for CRC value
        val = val << CRC7WIDTH;
        bit = bit << CRC7WIDTH;
        pol = pol << CRC7WIDTH;

        // Insert initial vector
        val |= CRC7IVEC;

        // Apply division until all bits done
        while (bit & (DATA7MASK << CRC7WIDTH))
        {
            if (bit & val)
            {
                val ^= pol;
            }

            bit >>= 1;
            pol >>= 1;
        }
        return val;
    }

    ENS21x::Result ENS21x::checkData(uint32_t data)
    {
        Result result = Result::STATUS_CRC_ERROR;

        data            &= 0xffffff;
        uint8_t valid    = (data>>16) & 0x01;
        uint32_t crc     = (data>>17) & 0x7f;
        uint32_t payload = data       & 0x1ffff;

        if (crc7(payload) == crc)
        {
            result = valid == 1 ? Result::STATUS_OK : Result::STATUS_INVALID;
        }

        debug(__func__, result);
        if (result != Result::STATUS_OK)
        {
            debug("Data   : ", data);
            debug("payload: ", payload);
            debug("CRC    : ", crc);
            debug("Valid  : ", valid);
        }

        return result;
    }
}