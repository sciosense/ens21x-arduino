#ifndef SCIOSENSE_ENS21X_DEFINES_C_H
#define SCIOSENSE_ENS21X_DEFINES_C_H

#include <inttypes.h>

//// Addresses of the ENS21x registers
typedef uint8_t ENS21x_RegisterAddress;
#define ENS21X_REGISTER_ADDRESS_PART_ID                         (0x00)      // Identifies the part as ENS21x
#define ENS21X_REGISTER_ADDRESS_DIE_REV                         (0x02)      // Identifies the die revision version
#define ENS21X_REGISTER_ADDRESS_UID                             (0x04)      // Unique identifier
#define ENS21X_REGISTER_ADDRESS_SYS_CTRL                        (0x10)      // System configuration
#define ENS21X_REGISTER_ADDRESS_SYS_STAT                        (0x11)      // System status
#define ENS21X_REGISTER_ADDRESS_SENS_RUN                        (0x21)      // The run mode (single shot or continuous)
#define ENS21X_REGISTER_ADDRESS_SENS_START                      (0x22)      // Start measurement
#define ENS21X_REGISTER_ADDRESS_SENS_STOP                       (0x23)      // Stop continuous measurement
#define ENS21X_REGISTER_ADDRESS_SENS_STAT                       (0x24)      // Sensor status (idle or measuring)
#define ENS21X_REGISTER_ADDRESS_T_VAL                           (0x30)      // Temperature readout
#define ENS21X_REGISTER_ADDRESS_H_VAL                           (0x33)      // Relative humidity readout

//// system Status (SYS_STAT)
#define ENS21X_SYSTEM_ACTIVE                                    (1)         // The system power is in active state

//// System Control (SYS_CTRL)
typedef uint8_t ENS21x_SystemControl;
#define ENS21X_SYSTEM_CONTROL_DISABLE_LOW_POWER                 (0)
#define ENS21X_SYSTEM_CONTROL_ENABLE_LOW_POWER                  (1 << 0)
#define ENS21X_SYSTEM_CONTROL_RESET                             (1 << 7)

//// Sensor Configuration
typedef uint8_t ENS21x_SensorConfiguration;
#define ENS21X_SENSOR_CONFIGURATION_TEMPERATURE                 (1 << 0)
#define ENS21X_SENSOR_CONFIGURATION_HUMIDITY                    (1 << 1)
#define ENS21X_SENSOR_CONFIGURATION_TEMPERATURE_AND_HUMIDITY    (ENS21X_SENSOR_CONFIGURATION_TEMPERATURE | ENS21X_SENSOR_CONFIGURATION_HUMIDITY)

//// Product Family IDs
#define ENS210_PART_ID                                          ((uint16_t) 0x0210)
#define ENS211_PART_ID                                          ((uint16_t) 0x0211)
#define ENS212_PART_ID                                          ((uint16_t) 0x0212)
#define ENS213A_PART_ID                                         ((uint16_t) 0xa213)
#define ENS215_PART_ID                                          ((uint16_t) 0x0215)

//// System Timing Characteristics in ms
#define ENS21X_SYSTEM_TIMING_BOOTING                            (2)         // Booting time in ms (also after reset, or going to high power)
#define ENS210_SYSTEM_TIMING_CONVERSION_SINGLE_SHOT             (130)       // Conversion time in ms for single shot T/H measurement
#define ENS210_SYSTEM_TIMING_CONVERSION_CONTINUOUS              (238)       // Conversion time in ms for continuous T/H measurement
#define ENS211_SYSTEM_TIMING_CONVERSION_SINGLE_SHOT             (32)        // Conversion time in ms for single shot T/H measurement
#define ENS211_SYSTEM_TIMING_CONVERSION_CONTINUOUS              (59)        // Conversion time in ms for continuous T/H measurement
#define ENS212_SYSTEM_TIMING_CONVERSION_SINGLE_SHOT             (32)        // Conversion time in ms for single shot T/H measurement
#define ENS212_SYSTEM_TIMING_CONVERSION_CONTINUOUS              (59)        // Conversion time in ms for continuous T/H measurement
#define ENS213A_SYSTEM_TIMING_CONVERSION_SINGLE_SHOT            (130)       // Conversion time in ms for single shot T/H measurement
#define ENS213A_SYSTEM_TIMING_CONVERSION_CONTINUOUS             (256)       // Conversion time in ms for continuous T/H measurement
#define ENS215_SYSTEM_TIMING_CONVERSION_SINGLE_SHOT             (130)       // Conversion time in ms for single shot T/H measurement
#define ENS215_SYSTEM_TIMING_CONVERSION_CONTINUOUS              (256)       // Conversion time in ms for continuous T/H measurement

//// BufferInfo defines internally used buffer and data indices and sizes (in bytes)
#define ENS21X_BUFFER_INFO_DATA_SIZE                            (6)
#define ENS21X_BUFFER_INFO_PART_ID_SIZE                         (2)
#define ENS21X_BUFFER_INFO_DIE_REV_SIZE                         (2)
#define ENS21X_BUFFER_INFO_UID_SIZE                             (8)

//// Result and Errors
#ifndef SCIOSENSE_RESULT_CODES
#define SCIOSENSE_RESULT_CODES
typedef int8_t Result;
#define RESULT_NOT_ALLOWED                                      (4)     // The requested command is not allowed.
#define RESULT_CHECKSUM_ERROR                                   (3)     // The value was read, but the checksum over the payload (valid and data) does not match.
#define RESULT_INVALID                                          (2)     // The value was read, but the data is invalid.
#define RESULT_IO_ERROR                                         (1)     // There was an IO communication error, read/write the stream failed.
#define RESULT_OK                                               (0)     // All OK; The value was read, the checksum matches, and data is valid.
#endif

#endif // SCIOSENSE_ENS21X_DEFINES_C_H