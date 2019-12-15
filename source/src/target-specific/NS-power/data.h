#ifndef _NS_DATA_H
#define _NS_DATA_H

// #include "NectarContract.h"
#include <stdint.h>

typedef enum {
    M_VOLTAGE,
    M_CURRENT,
    M_POWER,
    R_VOLTAGE,
    R_CURRENT,
    GRID_METER,
    SUN_METER,
    TIMESTAMP,
    STATUS,
    TEMPERATURE,
    MOM_DATA,
    REF_DATA,
    METER_DATA,
    POWER_TYPE_COUNT
} powerDataType_t;

typedef enum {
    GENERATOR_STATUS,
    MPPT_STATUS,
    PWM_STATUS,
    CALIBRATION_STATUS,
    OVERHEAT_STATUS,
    BOOST_STATUS,
    STATUS_TYPE_COUNT
} statusDataType_t;

typedef struct momentData_tag {
    uint16_t voltage;       // * 100
    uint16_t current;       // * 100
    uint32_t power;         // * 1000000
} momentData_t;

typedef struct referenceData_tag {
    uint32_t voltage;       // * 1000000000
    uint32_t current;       // * 1000000000
} referenceData_t;

typedef struct meterData_tag {
    uint32_t grid;          // Watt
    uint32_t sun;           // Watt
} meterData_t;

typedef struct PowerBoardDataStruct_tag {
    momentData_t mom;
    referenceData_t ref;
    meterData_t meter;
    time_t timestamp;
    uint8_t status;         // 0b - generator_status, 1b - MPPT_status, 2b - PWM_status, 3b - calibration_status, 4b - overheat_status
    uint8_t temperature;
} PowerBoardDataStruct_t;

uint8_t PowerData_init(void);
uint8_t *PowerData_getStatusPtr(void);
uint8_t PowerData_read(powerDataType_t datatype, void *d);
uint8_t PowerData_write(powerDataType_t datatype, void *d);
uint32_t PowerData_calculatePower(void);
uint8_t PowerData_info(void);

void SunStatus_set(uint8_t status);
uint8_t SunStatus_get(void);

#define CLEAR_STATUS(x)         (*PowerData_getStatusPtr() &= (uint8_t)~(1U << x))
#define SET_STATUS(x)           (*PowerData_getStatusPtr() |= (uint8_t)(1U << x))
#define GET_STATUS(x)           ((*PowerData_getStatusPtr() & (1U << x)) >> x)

// convert from float
#define T_CONVERT(x)            ((uint8_t)(x * 100))
#define VI_CONVERT(x)           ((uint16_t)(x * 100))
#define METER_CONVERT(x)        ((uint32_t)(x * 1000000))
#define POWER_CONVERT(x)        ((uint32_t)(x * 1000000))
#define REF_CONVERT(x)          ((uint32_t)(x * 1000000000))

// convert to float
#define DIV_T_CONVERT(x)        ((float)x / 100)
#define DIV_VI_CONVERT(x)       ((float)x / 100)
#define DIV_METER_CONVERT(x)    ((float)x / 1000000)
#define DIV_POWER_CONVERT(x)    ((float)x / 1000000)
#define DIV_REF_CONVERT(x)      ((float)x / 1000000000)

typedef struct {
    uint8_t isCalibrating;
    uint8_t isInOnboarding;
    uint8_t isTestMode;
    uint8_t current_state;

    uint32_t error;
    uint8_t readingSerial;
    uint8_t readingTemperature;
    uint8_t safeToReadTemp;
    uint8_t startCalibration;
} Data;

typedef struct {
    float ref_voltage;
    float ref_current;
    float sun_meter;
    float grid_meter;
} StorageData;

uint8_t data_getSafeToReadTemp(void);
void data_setSafeToReadTemp(uint8_t set);
uint8_t data_getReadingTemperature(void);
void data_setReadingTemperature(uint8_t set);
uint8_t data_getReadingSerial(void);
void data_setReadingSerial(uint8_t set);
uint8_t data_getStartCalibration(void);
void data_setStartCalibration(uint8_t set);
uint8_t data_getIsCalibrating(void);
void data_setIsCalibrating(uint8_t set);
uint8_t data_getIsInOnboarding(void);
void data_setIsInOnboarding(uint8_t set);
uint8_t data_getIsTestMode(void);
void data_setIsTestMode(uint8_t set);
uint8_t data_getCurrent_state(void);
void data_setCurrent_state(uint8_t set);

#define LED_ON            0
#define LED_OFF           1

enum codes {
    NS_OK,
    NS_ERROR,
    MSG_OK,
    MSG_ERROR
};

enum modes {
    IDLE,
    RUNNING,
    STOP,
    MANUAL
};

// extern Data data;
extern StorageData storage_data;
// extern nectar_contract::PowerBoardState powerState;

#endif
