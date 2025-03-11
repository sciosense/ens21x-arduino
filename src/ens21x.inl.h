#include <Arduino.h>
#include <Wire.h>

#include "ens21x.h"


inline ENS21x::~ENS21x() { }

inline ENS21x::ENS21x()
{
    solderCorrection    = 0;
    partId              = 0;
    dieRev              = 0;
    uid                 = 0;

    debugStream         = nullptr;
}

inline void ENS21x::begin(TwoWire* wire, uint8_t address)
{
    i2cConfig           = { 0 };
    i2cConfig.wire      = wire;
    i2cConfig.address   = address;

    io.read             = ScioSense_Arduino_I2c_Read;
    io.write            = ScioSense_Arduino_I2c_Write;
    io.wait             = ScioSense_Arduino_I2c_Wait;
    io.config           = &i2cConfig;
}

inline bool ENS21x::isConnected()
{
    return Ens21x_isConnected(this);
}

bool ENS21x::init()
{
    Ens21x_Init(this);
    debug("PART_ID  : ", partId);

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

Result ENS21x::update(ENS21x_SensorConfiguration sensor)
{
    Result result;
    result = Ens21x_Update(this, sensor);

    debug(__func__, result);
    return result;
}

Result ENS21x::singleShotMeasure(ENS21x_SensorConfiguration sensor)
{
    Result result;
    result = Ens21x_SingleShotMeasure(this, sensor);

    debug(__func__, result);
    return result;
}

Result ENS21x::startContinuousMeasure(ENS21x_SensorConfiguration sensor)
{
    Result result;
    result = Ens21x_StartContinuousMeasure(this, sensor);

    debug(__func__, result);
    return result;
}

Result ENS21x::stopContinuousMeasure(ENS21x_SensorConfiguration sensor)
{
    Result result;
    result = Ens21x_StopContinuousMeasure(this, sensor);

    debug(__func__, result);
    return result;
}

Result ENS21x::setLowPower(bool enable)
{
    return Ens21x_SetLowPower(this, enable);
}

Result ENS21x::reset()
{
    Result result;
    result = Ens21x_Reset(this);

    debug(__func__, result);
    return result;
}

void ENS21x::wait()
{
    Ens21x_Wait(this);
}

uint16_t ENS21x::getPartId()
{
    return Ens21x_GetPartId(this);
}

uint16_t ENS21x::getDieRev()
{
    return Ens21x_GetDieRev(this);
}

uint64_t ENS21x::getUid()
{
    return Ens21x_GetUid(this);
}

void ENS21x::setSolderCorrection(int16_t correction)
{
   Ens21x_SetSolderCorrection(this, correction);
}

float ENS21x::getTempKelvin()
{
    return Ens21x_GetTempKelvin(this);
}

float ENS21x::getTempCelsius()
{
    return Ens21x_GetTempCelsius(this);
}

float ENS21x::getTempFahrenheit()
{
    return Ens21x_GetTempFahrenheit(this);
}

float ENS21x::getHumidityPercent()
{
    return Ens21x_GetHumidityPercent(this);
}

float ENS21x::getAbsoluteHumidityPercent()
{
    return Ens21x_GetAbsoluteHumidityPercent(this);
}

uint16_t ENS21x::getDataT()
{
    return Ens21x_GetDataT(this);
}

uint16_t ENS21x::getDataH()
{
    return Ens21x_GetDataH(this);
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
inline void ENS21x::debug(const char* msg, T data)
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
        case RESULT_IO_ERROR:       return "io-error";
        case RESULT_CHECKSUM_ERROR: return "crc-error";
        case RESULT_INVALID:        return "data-invalid";
        case RESULT_OK:             return "ok";
        default:                    return "unknown-status";
    }
}