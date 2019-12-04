#ifndef _NS_DATA_H
#define _NS_DATA_H

#include "NectarContract.h"

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
} powerDataType_t;

typedef enum {
    GENERATOR_STATUS,
    MPPT_STATUS,
    PWM_STATUS,
    CALIBRATION_STATUS,
    OVERHEAT_STATUS,
    BOOST_STATUS,
} statusDataType_t;

typedef struct momentData_tag {
    uint16_t voltage;       // * 100
    uint16_t current;       // * 100
    uint32_t power;         // * 1000000
} momentData_t;

typedef struct referenceData_tag {
    uint16_t voltage;       // * 100
    uint16_t current;       // * 100
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
uint8_t PowerData_info(void);

#define CLEAR_STATUS(x) (*PowerData_getStatusPtr() &= ~(1U << x))
#define SET_STATUS(x) (*PowerData_getStatusPtr() |= (1U << x))
#define GET_STATUS(x) ((*PowerData_getStatusPtr() & (1U << x)) >> x)


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

typedef struct {
    bool isCalibrating;
    bool isInOnboarding;
    bool isTestMode;
    uint8_t current_state;

    float moment_power;
    float moment_voltage;
    float moment_current;

    float reference_voltage;
    float reference_current;

    float grid_energy_meter_kwh;
    float sun_energy_meter_kwh;

    float pwm_duty;
    float device_temperature;

    bool mosfet_overheat_on;
    bool calibrated;
    bool generator_on;

    uint32_t error;
    bool readingSerial;
    bool readingTemperature;
    bool safeToReadTemp;
    bool startCalibration;
} Data;

typedef struct {
    float ref_voltage;
    float ref_current;
    float sun_meter;
    float grid_meter;
} StorageData;

extern Data data;
extern StorageData storage_data;
extern nectar_contract::PowerBoardState powerState;

#endif
