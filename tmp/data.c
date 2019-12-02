#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "data.h"

static PowerBoardDataStruct_t power_data;

uint8_t PowerData_init() {
    memset(&power_data, 0, sizeof(power_data));
    return 1;
}

// returns pointer to power_data.status struct
uint8_t *PowerData_getStatusPtr(void) {
    return &power_data.status;
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
