#include <string.h>
#include <stdio.h>
#include "data.h"

static uint16_t V_H_THRESHOLD = VI_CONVERT(100);
static uint16_t V_L_THRESHOLD = VI_CONVERT(10);
static uint16_t I_THRESHOLD = VI_CONVERT(0.2f);

static PowerBoardDataStruct_t power_data;
static uint8_t sunStatus;

uint8_t PowerData_init() {
    memset(&power_data, 0, sizeof(power_data));
    sunStatus = 0;
    return 0;
}

// returns pointer to power_data.status struct
uint8_t *PowerData_getStatusPtr(void) {
    return &power_data.status;
}

uint8_t PowerData_read(powerDataType_t datatype, void *d) {
    switch (datatype) {
        case M_VOLTAGE:
            *(uint16_t *)d = power_data.mom.voltage;
            break;

        case M_CURRENT:
            *(uint16_t *)d = power_data.mom.current;
            break;

        case M_POWER:
            *(uint32_t *)d = power_data.mom.power;
            break;

        case R_VOLTAGE:
            *(uint32_t *)d = power_data.ref.voltage;
            break;

        case R_CURRENT:
            *(uint32_t *)d = power_data.ref.current;
            break;

        case GRID_METER:
            *(uint32_t *)d = power_data.meter.grid;
            break;

        case SUN_METER:
            *(uint32_t *)d = power_data.meter.sun;
            break;

        case TIMESTAMP:
            *(time_t *)d = power_data.timestamp;
            break;

        case STATUS:
            *(uint8_t *)d = power_data.status;
            break;

        case TEMPERATURE:
            *(uint8_t *)d = power_data.temperature;
            break;

        case MOM_DATA:
            *(momentData_t *)d = power_data.mom;
            break;

        case REF_DATA:
            *(referenceData_t *)d = power_data.ref;
            break;

        case METER_DATA:
            *(meterData_t *)d = power_data.meter;
            break;

        case POWER_TYPE_COUNT:
        default:
            break;
    }
    return 0;
}

uint8_t PowerData_write(powerDataType_t datatype, void *d) {
    switch (datatype) {
        case M_VOLTAGE:
            power_data.mom.voltage = *(uint16_t *)d;
            // printf("M_VOLTAGE\n\r");
            if (power_data.mom.voltage >= V_H_THRESHOLD) {
                SET_STATUS(V_READY_STATUS);
            } else if (power_data.mom.voltage < V_L_THRESHOLD) {
                CLEAR_STATUS(V_READY_STATUS);
            }
            break;

        case M_CURRENT:
            power_data.mom.current = *(uint16_t *)d;
            // printf("M_CURRENT\n\r");
            if (power_data.mom.current >= I_THRESHOLD) {
                SET_STATUS(I_READY_STATUS);
            } else {
                CLEAR_STATUS(I_READY_STATUS);
            }
            break;

        case M_POWER:
            power_data.mom.power = *(uint32_t *)d;
            break;

        case R_VOLTAGE:
            power_data.ref.voltage = *(uint32_t *)d;
            break;

        case R_CURRENT:
            power_data.ref.current = *(uint32_t *)d;
            break;

        case GRID_METER:
            power_data.meter.grid = *(uint32_t *)d;
            break;

        case SUN_METER:
            power_data.meter.sun = *(uint32_t *)d;
            break;

        case TIMESTAMP:
            power_data.timestamp = *(time_t *)d;
            break;

        case STATUS:
            power_data.status = *(uint8_t *)d;
            break;

        case TEMPERATURE:
            power_data.temperature = *(uint8_t *)d;
            break;

        case MOM_DATA:
            power_data.mom = *(momentData_t *)d;
            break;

        case REF_DATA:
            power_data.ref = *(referenceData_t *)d;
            break;

        case METER_DATA:
            power_data.meter = *(meterData_t *)d;
            break;

        case POWER_TYPE_COUNT:
        default:
            break;
    }
    return 0;
}

uint32_t PowerData_calculatePower(void) {
    power_data.mom.power = (uint32_t)(power_data.mom.voltage * power_data.mom.current);
    return power_data.mom.power;
}

uint8_t PowerData_info(void) {
    printf("\n\r");
    printf("[PWR DATA] ***** INFO *****\n\r");
    printf("[PWR DATA] timestamp:           %lu\n\r", (long unsigned)power_data.timestamp);
    printf("[PWR DATA] moment voltage:      %d V\n\r", power_data.mom.voltage);
    printf("[PWR DATA] moment current:      %d A\n\r", power_data.mom.current);
    printf("[PWR DATA] moment power:        %u W\n\r", (unsigned int)power_data.mom.power);
    printf("[PWR DATA] reference voltage:   %u V\n\r", (unsigned int)power_data.ref.voltage);
    printf("[PWR DATA] reference current:   %u A\n\r", (unsigned int)power_data.ref.current);
    printf("[PWR DATA] meter grid:          %u kWh\n\r", (unsigned int)power_data.meter.grid);
    printf("[PWR DATA] meter sun:           %u kWh\n\r", (unsigned int)power_data.meter.sun);
    printf("[PWR DATA] GENERATOR_STATUS:    %d\n\r", GET_STATUS(GENERATOR_STATUS));
    printf("[PWR DATA] MPPT_STATUS:         %d\n\r", GET_STATUS(MPPT_STATUS));
    printf("[PWR DATA] PWM_STATUS:          %d\n\r", GET_STATUS(PWM_STATUS));
    printf("[PWR DATA] CALIBRATION_STATUS:  %d\n\r", GET_STATUS(CALIBRATION_STATUS));
    printf("[PWR DATA] BOOST_STATUS:        %d\n\r", GET_STATUS(BOOST_STATUS));
    printf("[PWR DATA] V_READY_STATUS:      %d\n\r", GET_STATUS(V_READY_STATUS));
    printf("[PWR DATA] I_READY_STATUS:      %d\n\r", GET_STATUS(I_READY_STATUS));
    printf("[PWR DATA] SUN_STATUS:          %d\n\r", GET_STATUS(SUN_STATUS));
    printf("[PWR DATA] OVERHEAT_STATUS:     %d\n\r", GET_STATUS(OVERHEAT_STATUS));
    // printf("[PWR DATA] status:              %d\n\r", power_data.status);
    printf("[PWR DATA] internal temp:       %d\n\r", power_data.temperature);
    return 0;
}

// void SunStatus_set(uint8_t status) {
//     sunStatus = status;
//     if (status) {
//         SET_STATUS(SUN_STATUS);
//     } else {
//         CLEAR_STATUS(SUN_STATUS);
//     }
// }

// uint8_t SunStatus_get(void) {
//     return sunStatus;
// }


Data data = {
    0, 0, 0, IDLE, 0, 0, 0, 0, 0
};

StorageData storage_data = {
    0.0, 0.0, 0.0, 0.0
};

uint8_t data_getSafeToReadTemp(void) {
    return data.safeToReadTemp;
}

void data_setSafeToReadTemp(uint8_t set) {
    data.safeToReadTemp = set;
}

//  uint32_t error;

uint8_t data_getReadingTemperature(void) {
    return data.readingTemperature;
}

void data_setReadingTemperature(uint8_t set) {
    data.readingTemperature = set;
}

uint8_t data_getReadingSerial(void) {
    return data.readingSerial;
}

void data_setReadingSerial(uint8_t set) {
    data.readingSerial = set;
}

uint8_t data_getStartCalibration(void) {
    return data.startCalibration;
}

void data_setStartCalibration(uint8_t set) {
    data.startCalibration = set;
}

uint8_t data_getIsCalibrating(void) {
    return data.isCalibrating;
}

void data_setIsCalibrating(uint8_t set) {
    data.isCalibrating = set;
}

uint8_t data_getIsInOnboarding(void) {
    return data.isInOnboarding;
}

void data_setIsInOnboarding(uint8_t set) {
    data.isInOnboarding = set;
}

uint8_t data_getIsTestMode(void) {
    return data.isTestMode;
}

void data_setIsTestMode(uint8_t set) {
    data.isTestMode = set;
}

uint8_t data_getCurrent_state(void) {
    return data.current_state;
}

void data_setCurrent_state(uint8_t set) {
    data.current_state = set;
}

// static const uint8_t isCalibrating = 0;
// static const uint8_t isInOnboarding = 1;
// static const uint8_t isTestMode = 0;
// static const uint8_t current_state = IDLE;

// // change to uint16_t
// // should perform all calculations in uint32_t cast to uint16_t

// static const float moment_power = 0.00;
// static const float moment_voltage = 0.00;
// static const float moment_current = 0.00;

// static const float reference_voltage = 0.00;
// static const float reference_current = 0.00;

// static const float grid_energy_meter_kwh = 0.00;
// static const float sun_energy_meter_kwh = 0.00;

// static const float pwm_duty = 0.1f;
// static const float device_temperature = 0.00;

// static const uint8_t mosfet_overheat_on = 0;
// static const uint8_t calibrated = 0;
// static const uint8_t generator_on = 0;

// static const uint32_t power_board_error = 0x00000000;
// static const uint8_t readingSerial = 0;
// static const uint8_t readingTemperature = 0;
// static const uint8_t safeToReadTemp = 0;
// static const uint8_t startCalibration = 0;