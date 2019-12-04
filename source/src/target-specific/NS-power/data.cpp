// #include "mbed.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "data.h"

static PowerBoardDataStruct_t power_data;

uint8_t PowerData_init() {
    memset(&power_data, 0, sizeof(power_data));
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
            *(uint16_t *)d = power_data.ref.voltage;
        break;
        
        case R_CURRENT:
            *(uint16_t *)d = power_data.ref.current;
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

        default:
        break;
    }
    return 0;
}

uint8_t PowerData_write(powerDataType_t datatype, void *d) {
    switch(datatype) {
        case M_VOLTAGE:
            power_data.mom.voltage = *(uint16_t *)d;
        break;

        case M_CURRENT:
            power_data.mom.current = *(uint16_t *)d;
        break;

        case M_POWER:
            power_data.mom.power = *(uint32_t *)d;
        break;
    
        case R_VOLTAGE:
            power_data.ref.voltage = *(uint16_t *)d;
        break;
        
        case R_CURRENT:
            power_data.ref.current = *(uint16_t *)d;
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

        default:
        break;
    }
    return 0;
}

uint8_t PowerData_info(void) {
    printf("[PWR DATA] ***** INFO *****\n\r");
    printf("[PWR DATA] timestamp:           %lu\n\r", (long unsigned)power_data.timestamp);
    printf("[PWR DATA] moment voltage:      %d V\n\r", power_data.mom.voltage);
    printf("[PWR DATA] moment current:      %d A\n\r", power_data.mom.current);
    printf("[PWR DATA] moment power:        %u W\n\r", (unsigned int)power_data.mom.power);
    printf("[PWR DATA] reference voltage:   %d V\n\r", power_data.ref.voltage);
    printf("[PWR DATA] reference current:   %d A\n\r", power_data.ref.current);
    printf("[PWR DATA] meter grid:          %u kWh\n\r", (unsigned int)power_data.meter.grid);
    printf("[PWR DATA] meter sun:           %u kWh\n\r", (unsigned int)power_data.meter.sun);
    printf("[PWR DATA] GENERATOR_STATUS:    %d\n\r", GET_STATUS(GENERATOR_STATUS));
    printf("[PWR DATA] MPPT_STATUS:         %d\n\r", GET_STATUS(MPPT_STATUS));
    printf("[PWR DATA] PWM_STATUS:          %d\n\r", GET_STATUS(PWM_STATUS));
    printf("[PWR DATA] CALIBRATION_STATUS:  %d\n\r", GET_STATUS(CALIBRATION_STATUS));
    printf("[PWR DATA] OVERHEAT_STATUS:     %d\n\r", GET_STATUS(OVERHEAT_STATUS));
    printf("[PWR DATA] BOOST_STATUS:        %d\n\r", GET_STATUS(BOOST_STATUS));
    // printf("[PWR DATA] status:              %d\n\r", power_data.status);
    printf("[PWR DATA] internal temp:       %d\n\r", power_data.temperature);
    return 0;
}

bool isCalibrating = false;
bool isInOnboarding = true;
bool isTestMode = false;
uint8_t current_state = IDLE;

// change to uint16_t
// should perform all calculations in uint32_t cast to uint16_t
  
float moment_power = 0.00;
float moment_voltage = 0.00;
float moment_current = 0.00;

float reference_voltage = 0.00;
float reference_current = 0.00;
  
float grid_energy_meter_kwh = 0.00;
float sun_energy_meter_kwh = 0.00;

float pwm_duty = 0.1f;
float device_temperature = 0.00;

bool mosfet_overheat_on = false;
bool calibrated = false;
bool generator_on = false;
  
uint32_t power_board_error = 0x00000000;
bool readingSerial = false;
bool readingTemperature = false;
bool safeToReadTemp = false;
bool startCalibration = false;

Data data = {
  isCalibrating, isInOnboarding, isTestMode, current_state, moment_power, moment_voltage, moment_current, reference_voltage, reference_current, grid_energy_meter_kwh, sun_energy_meter_kwh,
  pwm_duty, device_temperature, mosfet_overheat_on, calibrated, generator_on, power_board_error, readingSerial, readingTemperature, safeToReadTemp, startCalibration
};

StorageData storage_data = {
  reference_voltage, reference_current, sun_energy_meter_kwh, grid_energy_meter_kwh
};

nectar_contract::PowerBoardState powerState = {
  0,
  0,
  0,
  0.1f,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0.0f,
  0.0f,
  0.0f
};
