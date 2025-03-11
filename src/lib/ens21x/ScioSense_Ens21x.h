#ifndef SCIOSENSE_ENS21X_C_H
#define SCIOSENSE_ENS21X_C_H

#include "ScioSense_Ens21x_defines.h"

#include <stdbool.h>
#include <inttypes.h>

typedef struct ScioSense_Ens21x_IO
{
    Result  (*read)     (void* config, const uint16_t address, uint8_t* data, const size_t size);
    Result  (*write)    (void* config, const uint16_t address, uint8_t* data, const size_t size);
    void    (*wait)     (const uint32_t ms);
    void* config;
} ScioSense_Ens21x_IO;

typedef struct ScioSense_Ens21x
{
    ScioSense_Ens21x_IO         io;
    uint8_t                     dataBuffer      [ENS21X_BUFFER_INFO_DATA_SIZE];
    int16_t                     solderCorrection;
    uint16_t                    partId;
    uint16_t                    dieRev;
    uint64_t                    uid;
} ScioSense_Ens21x;

static inline Result Ens21x_Init                      (ScioSense_Ens21x* ens21x);             // resets the device and reads IDs
static inline Result Ens21x_Reset                     (ScioSense_Ens21x* ens21x);             // reset the device
static inline Result Ens21x_Update                    (ScioSense_Ens21x* ens21x, ENS21x_SensorConfiguration sensor);
static inline Result Ens21x_ReadPartId                (ScioSense_Ens21x* ens21x);             // Reads the PART_ID; Use Ens21x_GetPartId to get the value
static inline Result Ens21x_ReadDieRev                (ScioSense_Ens21x* ens21x);             // Reads the DIE_REV; Use Ens21x_GetDieRev to get the value
static inline Result Ens21x_ReadUid                   (ScioSense_Ens21x* ens21x);             // Reads the UID; Use Ens21x_GetUid to get the value
static inline Result Ens21x_SingleShotMeasure         (ScioSense_Ens21x* ens21x, ENS21x_SensorConfiguration sensor);   // invokes a single shot mode measuring for the given sensor
static inline Result Ens21x_StartContinuousMeasure    (ScioSense_Ens21x* ens21x, ENS21x_SensorConfiguration sensor);   // starts the continuous measure mode for the given sensor
static inline Result Ens21x_StopContinuousMeasure     (ScioSense_Ens21x* ens21x, ENS21x_SensorConfiguration sensor);   // stops the continuous measure mode for the given sensor
static inline Result Ens21x_SetLowPower               (ScioSense_Ens21x* ens21x, bool enable);// en-/disables the low power mode
static inline void   Ens21x_Wait                      (ScioSense_Ens21x* ens21x);             // Waits for continuous measure to finish
void   Ens21x_WaitSingleShot            (ScioSense_Ens21x* ens21x);             // Waits for continuous measure to finish

static inline void  Ens21x_SetSolderCorrection        (ScioSense_Ens21x* ens21x, int16_t correction); // Sets the solder correction (default is 50mK) - used to calculate temperature and humidity values
static inline float Ens21x_GetTempKelvin              (ScioSense_Ens21x* ens21x);             // Converts and returns measurement data in Kelvin
static inline float Ens21x_GetTempCelsius             (ScioSense_Ens21x* ens21x);             // Converts and returns measurement data in Celsius
static inline float Ens21x_GetTempFahrenheit          (ScioSense_Ens21x* ens21x);             // Converts and returns measurement data in Fahrenheit
static inline float Ens21x_GetHumidityPercent         (ScioSense_Ens21x* ens21x);             // Converts and returns measurement data in %RH
static inline float Ens21x_GetAbsoluteHumidityPercent (ScioSense_Ens21x* ens21x);             // Converts and returns measurement data in %aH

static inline uint16_t Ens21x_GetDataT                (ScioSense_Ens21x* ens21x);             // returns raw temperature data; e.g. to be used as temperature compensation for ENS160 (TEMP_IN)
static inline uint16_t Ens21x_GetDataH                (ScioSense_Ens21x* ens21x);             // returns raw humidity data; e.g. to be used as humidity compensation for ENS160 (RH_IN)
static inline uint16_t Ens21x_GetPartId               (ScioSense_Ens21x* ens21x);             // returns the PART_ID or zero if invalid
static inline uint16_t Ens21x_GetDieRev               (ScioSense_Ens21x* ens21x);             // returns the DIE_REV or zero if invalid
static inline uint64_t Ens21x_GetUid                  (ScioSense_Ens21x* ens21x);             // returns the UID or zero if invalid

static inline bool Ens21x_isConnected                 (ScioSense_Ens21x* ens21x);             // checks if the read PART_ID matches the expected value; returns true, if so.
static inline bool Ens210_isConnected                 (ScioSense_Ens21x* ens21x);             // checks if the read PART_ID matches the expected value; returns true, if so.
static inline bool Ens211_isConnected                 (ScioSense_Ens21x* ens21x);             // checks if the read PART_ID matches the expected value; returns true, if so.
static inline bool Ens212_isConnected                 (ScioSense_Ens21x* ens21x);             // checks if the read PART_ID matches the expected value; returns true, if so.
static inline bool Ens213a_isConnected                (ScioSense_Ens21x* ens21x);             // checks if the read PART_ID matches the expected value; returns true, if so.
static inline bool Ens215_isConnected                 (ScioSense_Ens21x* ens21x);             // checks if the read PART_ID matches the expected value; returns true, if so.


#include "ScioSense_Ens21x.inl.h"
#endif // SCIOSENSE_ENS21X_C_H