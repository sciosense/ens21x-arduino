#ifndef SCIOSENSE_ENS21X_INL_C_H
#define SCIOSENSE_ENS21X_INL_C_H

#include <math.h>

#include "ScioSense_Ens21x.h"

#define read(address, data, len)    ens21x->io.read (ens21x->io.config, (address), (uint8_t*)(data), (len))
#define write(address, data, len)   ens21x->io.write(ens21x->io.config, (address), (uint8_t*)(data), (len))
#define wait(ms)                    ens21x->io.wait(ms)

#define hasAnyFlag(a, b)            (((a) & (b)) != 0)
#define hasFlag(a, b)               (((a) & (b)) == (b))

static inline uint32_t    Ens21x_Crc7(uint32_t val);
static inline Result      Ens21x_CheckData(uint8_t* data);
static inline Result      Ens21x_ReadIdentifiers(ScioSense_Ens21x* ens21x);

static inline uint16_t Ens21x_GetValueOf16(const uint8_t* data)
{
    return ((uint16_t)data[1] << 8) + (uint16_t)data[0];
}

static inline uint32_t Ens21x_GetValueOf32(uint8_t* data)
{
    return  ((uint32_t)data[3] << 24)
          + ((uint32_t)data[2] << 16)
          + ((uint32_t)data[1] <<  8)
          +  (uint32_t)data[0];
}

static inline uint64_t Ens21x_GetValueOf64(uint8_t* data)
{
    return  ((uint64_t)data[7] << 56)
          + ((uint64_t)data[6] << 48)
          + ((uint64_t)data[5] << 40)
          + ((uint64_t)data[4] << 32)
          + ((uint64_t)data[3] << 24)
          + ((uint64_t)data[2] << 16)
          + ((uint64_t)data[1] << 8)
          +  (uint64_t)data[0];
}

static inline Result Ens21x_Init(ScioSense_Ens21x* ens21x)
{
    Result result;

    result = Ens21x_Reset(ens21x);
    if (result != RESULT_OK)
    {
        return result;
    }

    return Ens21x_ReadIdentifiers(ens21x);
}

static inline Result Ens21x_Reset(ScioSense_Ens21x* ens21x)
{
    Result result;
    static ENS21x_SystemControl reset = ENS21X_SYSTEM_CONTROL_RESET;

    result = write(ENS21X_REGISTER_ADDRESS_SYS_CTRL, &reset, sizeof(reset));
    if (result == RESULT_OK)
    {
        wait(ENS21X_SYSTEM_TIMING_BOOTING);
    }

    return result;
}

static inline Result Ens21x_Update(ScioSense_Ens21x* ens21x, ENS21x_SensorConfiguration sensor)
{
    Result result;
    result = read(ENS21X_REGISTER_ADDRESS_T_VAL, ens21x->dataBuffer, ENS21X_BUFFER_INFO_DATA_SIZE);
    if (result == RESULT_OK)
    {
        if (hasFlag(sensor, ENS21X_SENSOR_CONFIGURATION_TEMPERATURE))
        {
            result = Ens21x_CheckData(ens21x->dataBuffer);
        }

        if (   result == RESULT_OK
            && hasFlag(sensor, ENS21X_SENSOR_CONFIGURATION_HUMIDITY))
        {
            return Ens21x_CheckData(ens21x->dataBuffer + 3);
        }
    }

    return result;
}

static inline Result Ens21x_ReadPartId(ScioSense_Ens21x* ens21x)
{
    Result result;
    uint8_t data[ENS21X_BUFFER_INFO_PART_ID_SIZE];

    result = read(ENS21X_REGISTER_ADDRESS_PART_ID, data, sizeof(data));
    if (result == RESULT_OK)
    {
        ens21x->partId = Ens21x_GetValueOf16(data);
    }

    return result;
}

static inline Result Ens21x_ReadDieRev(ScioSense_Ens21x* ens21x)
{
    Result result;
    uint8_t data[ENS21X_BUFFER_INFO_DIE_REV_SIZE];

    result = read(ENS21X_REGISTER_ADDRESS_DIE_REV, data, sizeof(data));
    if (result == RESULT_OK)
    {
        ens21x->dieRev = Ens21x_GetValueOf16(data);
    }

    return result;
}

static inline Result Ens21x_ReadUid(ScioSense_Ens21x* ens21x)
{
    Result result;
    uint8_t data[ENS21X_BUFFER_INFO_UID_SIZE];

    result = read(ENS21X_REGISTER_ADDRESS_UID, data, sizeof(data));
    if (result == RESULT_OK)
    {
        ens21x->uid = Ens21x_GetValueOf64(data);
    }

    return result;
}

static inline Result Ens21x_ReadIdentifiers(ScioSense_Ens21x* ens21x)
{
    Result result;

    ens21x->partId  = 0;
    ens21x->dieRev  = 0;
    ens21x->uid     = 0;

    result = Ens21x_SetLowPower(ens21x, false);

    if (result == RESULT_OK)
    {
        result = Ens21x_ReadPartId(ens21x);
        if (result != RESULT_OK)
        {
            return result;
        }

        result = Ens21x_ReadDieRev(ens21x);
        if (result != RESULT_OK)
        {
            return result;
        }

        result = Ens21x_ReadUid(ens21x);
        if (result != RESULT_OK)
        {
            return result;
        }
    }

    result = Ens21x_SetLowPower(ens21x, true);

    return result;
}

static inline Result Ens21x_SingleShotMeasure(ScioSense_Ens21x* ens21x, ENS21x_SensorConfiguration sensor)
{
    Result result;

    result = write(ENS21X_REGISTER_ADDRESS_SENS_START, &sensor, sizeof(sensor));
    if (result == RESULT_OK)
    {
        Ens21x_WaitSingleShot(ens21x);
        result = Ens21x_Update(ens21x, sensor);
    }

    return result;
}

static inline Result Ens21x_StartContinuousMeasure(ScioSense_Ens21x* ens21x, ENS21x_SensorConfiguration sensor)
{
    Result result;

    result = write(ENS21X_REGISTER_ADDRESS_SENS_RUN, &sensor, sizeof(sensor));
    if (result == RESULT_OK)
    {
        result = Ens21x_SingleShotMeasure(ens21x, sensor);
    }

    return result;
}

static inline Result Ens21x_StopContinuousMeasure(ScioSense_Ens21x* ens21x, ENS21x_SensorConfiguration sensor)
{
    return write(ENS21X_REGISTER_ADDRESS_SENS_STOP, &sensor, sizeof(sensor));
}

static inline Result Ens21x_SetLowPower(ScioSense_Ens21x* ens21x, bool enable)
{
    Result  result;
    uint8_t sysStat;

    ENS21x_SystemControl sc = enable ? ENS21X_SYSTEM_CONTROL_ENABLE_LOW_POWER : ENS21X_SYSTEM_CONTROL_DISABLE_LOW_POWER;
    result                  = write(ENS21X_REGISTER_ADDRESS_SYS_CTRL, &sc, sizeof(sc));
    if (result == RESULT_OK)
    {
        wait(ENS21X_SYSTEM_TIMING_BOOTING);

        result = read(ENS21X_REGISTER_ADDRESS_SYS_STAT, &sysStat, 1);
        if (result == RESULT_OK)
        {
            if (enable == (sysStat == ENS21X_SYSTEM_ACTIVE))
            {
                result = RESULT_INVALID;
            }
        }
    }

    return result;
}

static inline void Ens21x_Wait(ScioSense_Ens21x* ens21x)
{
    switch (ens21x->partId)
    {
        case ENS210_PART_ID :   wait(ENS210_SYSTEM_TIMING_CONVERSION_CONTINUOUS);   break;
        case ENS211_PART_ID :   wait(ENS211_SYSTEM_TIMING_CONVERSION_CONTINUOUS);   break;
        case ENS212_PART_ID :   wait(ENS212_SYSTEM_TIMING_CONVERSION_CONTINUOUS);   break;
        case ENS213A_PART_ID:   wait(ENS213A_SYSTEM_TIMING_CONVERSION_CONTINUOUS);  break;
        case ENS215_PART_ID :   wait(ENS215_SYSTEM_TIMING_CONVERSION_CONTINUOUS);   break;

        default             :   wait(ENS215_SYSTEM_TIMING_CONVERSION_CONTINUOUS);   break;
    }
}

inline void Ens21x_WaitSingleShot(ScioSense_Ens21x* ens21x)
{
    switch (ens21x->partId)
    {
        case ENS210_PART_ID :   wait(ENS210_SYSTEM_TIMING_CONVERSION_SINGLE_SHOT);  break;
        case ENS211_PART_ID :   wait(ENS211_SYSTEM_TIMING_CONVERSION_SINGLE_SHOT);  break;
        case ENS212_PART_ID :   wait(ENS212_SYSTEM_TIMING_CONVERSION_SINGLE_SHOT);  break;
        case ENS213A_PART_ID:   wait(ENS213A_SYSTEM_TIMING_CONVERSION_SINGLE_SHOT); break;
        case ENS215_PART_ID :   wait(ENS215_SYSTEM_TIMING_CONVERSION_SINGLE_SHOT);  break;

        default             :   wait(ENS215_SYSTEM_TIMING_CONVERSION_SINGLE_SHOT);  break;
    }
}

static inline void Ens21x_SetSolderCorrection(ScioSense_Ens21x* ens21x, int16_t correction)
{
    ens21x->solderCorrection = correction;
}

static inline uint32_t Ens21x_Crc7(uint32_t val)
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

static inline Result Ens21x_CheckData(uint8_t* data)
{
    uint8_t  valid;
    uint8_t  crc;
    uint32_t payload;

    valid = data[2] & 0x01;
    if (valid == 1)
    {
        crc     = data[2] >> 1;
        payload = (uint32_t)Ens21x_GetValueOf16(data) | (uint32_t)0x10000;
        if (crc == Ens21x_Crc7(payload))
        {
            return RESULT_OK;
        }
        else
        {
            return RESULT_CHECKSUM_ERROR;
        }
    }

    return RESULT_INVALID;
}

static inline float Ens21x_GetTempKelvin(ScioSense_Ens21x* ens21x)
{
    return (Ens21x_GetDataT(ens21x) - ens21x->solderCorrection) / 64.f;
}

static inline float Ens21x_GetTempCelsius(ScioSense_Ens21x* ens21x)
{
    return Ens21x_GetTempKelvin(ens21x) - 273.15f;
}

static inline float Ens21x_GetTempFahrenheit(ScioSense_Ens21x* ens21x)
{
    return (9.f * (Ens21x_GetDataT(ens21x) - ens21x->solderCorrection) / 320.f) - 459.67f;
}

static inline float Ens21x_GetHumidityPercent(ScioSense_Ens21x* ens21x)
{
    return Ens21x_GetDataH(ens21x) / 512.f;
}

static inline float Ens21x_GetAbsoluteHumidityPercent(ScioSense_Ens21x* ens21x)
{
    static const float MOLAR_MASS_OF_WATER      = 18.01534f;
    static const float UNIVERSAL_GAS_CONSTANT   = 8.21447215f;

    float c = Ens21x_GetTempCelsius(ens21x);
    float h = Ens21x_GetHumidityPercent(ens21x);

    return (6.1121f * powf(2.718281828f, (17.67f * c) / (c + 243.5f)) * h * MOLAR_MASS_OF_WATER) / ((273.15f + c ) * UNIVERSAL_GAS_CONSTANT);
}

static inline uint16_t Ens21x_GetDataT(ScioSense_Ens21x* ens21x)
{
    return Ens21x_GetValueOf16(ens21x->dataBuffer);
}

static inline uint16_t Ens21x_GetDataH(ScioSense_Ens21x* ens21x)
{
    return Ens21x_GetValueOf16(ens21x->dataBuffer + 3);
}

static inline uint16_t Ens21x_GetPartId(ScioSense_Ens21x* ens21x)
{
    return ens21x->partId;
}

static inline uint16_t Ens21x_GetDieRev(ScioSense_Ens21x* ens21x)
{
    return ens21x->dieRev;
}

static inline uint64_t Ens21x_GetUid(ScioSense_Ens21x* ens21x)
{
    return ens21x->uid;
}

static inline bool Ens21x_isConnected(ScioSense_Ens21x* ens21x)
{
    return (ens21x->partId == 0xa213) || ((ens21x->partId >= 0x210) && (ens21x->partId <= 0x215));
}

static inline bool Ens210_isConnected(ScioSense_Ens21x* ens21x)
{
    return (ens21x->partId == ENS210_PART_ID);
}

static inline bool Ens211_isConnected(ScioSense_Ens21x* ens21x)
{
    return (ens21x->partId == ENS211_PART_ID);
}

static inline bool Ens212_isConnected(ScioSense_Ens21x* ens21x)
{
    return (ens21x->partId == ENS212_PART_ID);
}

static inline bool Ens213a_isConnected(ScioSense_Ens21x* ens21x)
{
    return (ens21x->partId == ENS213A_PART_ID);
}

static inline bool Ens215_isConnected(ScioSense_Ens21x* ens21x)
{
    return (ens21x->partId == ENS215_PART_ID);
}

#undef read
#undef write
#undef wait
#undef hasAnyFlag
#undef hasFlag

#endif // SCIOSENSE_ENS21X_INL_C_H