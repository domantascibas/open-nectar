#include "mbed.h"
#include "data.h"

PowerBoardDataStruct_t power_data;

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

float pwm_duty = 0.1;
float device_temperature = 0.00;

bool mosfet_overheat_on = false;
bool calibrated = false;
bool generator_on = false;
  
uint32_t power_board_error = 0x00000000;
bool readingSerial = false;
bool readingTemperature = false;
bool safeToReadTemp = false;
bool startCalibration = false;

uint8_t PowerData_init() {
    memset(&power_data, 0, sizeof(power_data));
    return 1;
}

uint8_t PowerData_read(powerDataType_t datatype, void *data) {
    switch (datatype) {
        case M_VOLTAGE:
            *(uint16_t *)data = power_data.mom.voltage;
        break;

        case M_CURRENT:
            *(uint16_t *)data = power_data.mom.current;
        break;

        case M_POWER:
            *(uint32_t *)data = power_data.mom.power;
        break;
    
        case R_VOLTAGE:
            *(uint16_t *)data = power_data.ref.voltage;
        break;
        
        case R_CURRENT:
            *(uint16_t *)data = power_data.ref.current;
        break;
        
        case GRID_METER:
            *(uint32_t *)data = power_data.meter.grid;
        break;
        
        case SUN_METER:
            *(uint32_t *)data = power_data.meter.sun;
        break;
        
        case TIMESTAMP:
            *(time_t *)data = power_data.timestamp;
        break;
        
        case STATUS:
            *(uint8_t *)data = power_data.status;
        break;
        
        case TEMPERATURE:
            *(uint8_t *)data = power_data.temperature;
        break;
        
        case MOM_DATA:
            *(momentData_t *)data = power_data.mom;
        break;
        
        case REF_DATA:
            *(referenceData_t *)data = power_data.ref;
        break;
        
        case METER_DATA:
            *(meterData_t *)data = power_data.meter;
        break;
    }
    return 1;
}

uint8_t PowerData_write(powerDataType_t datatype, void *data) {
    switch(datatype) {
        case M_VOLTAGE:
            power_data.mom.voltage = *(uint16_t *)data;
        break;

        case M_CURRENT:
            power_data.mom.current = *(uint16_t *)data;
        break;

        case M_POWER:
            power_data.mom.power = *(uint32_t *)data;
        break;
    
        case R_VOLTAGE:
            power_data.ref.voltage = *(uint16_t *)data;
        break;
        
        case R_CURRENT:
            power_data.ref.current = *(uint16_t *)data;
        break;
        
        case GRID_METER:
            power_data.meter.grid = *(uint32_t *)data;
        break;
        
        case SUN_METER:
            power_data.meter.sun = *(uint32_t *)data;
        break;
        
        case TIMESTAMP:
            power_data.timestamp = *(time_t *)data;
        break;
        
        case STATUS:
            power_data.status = *(uint8_t *)data;
        break;
        
        case TEMPERATURE:
            power_data.temperature = *(uint8_t *)data;
        break;
        
        case MOM_DATA:
            power_data.mom = *(momentData_t *)data;
        break;
        
        case REF_DATA:
            power_data.ref = *(referenceData_t *)data;
        break;
        
        case METER_DATA:
            power_data.meter = *(meterData_t *)data;
        break;
    }
    return 1;
}

Data data = {
  isCalibrating, isInOnboarding, isTestMode, current_state, moment_power, moment_voltage, moment_current, reference_voltage, reference_current, grid_energy_meter_kwh, sun_energy_meter_kwh,
  pwm_duty, device_temperature, mosfet_overheat_on, calibrated, generator_on, power_board_error, readingSerial, readingTemperature, safeToReadTemp
};

StorageData storage_data = {
  reference_voltage, reference_current, sun_energy_meter_kwh, grid_energy_meter_kwh
};

nectar_contract::PowerBoardState powerState = {
  0,
  0,
  0,
  0.1,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};
