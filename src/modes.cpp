#include "mbed.h"
#include "modes.h"
#include "mode_default.h"
#include "utils.h"

//currently defaults. Must initialize them from ESP
uint8_t mode_current = MODE_DEFAULT;
double temperature_max;
double temperature_set;
double temperature_current;
bool pv_available = false;

void device::set_mode(uint8_t mode_new) {
    mode_current = mode_new;
}

uint8_t device::get_mode() {
    return mode_current;
}

void device::run(uint8_t mode) {
    switch(mode) {
        default:  //mode is any other value than the ones below
        case MODE_DEFAULT:
            printf("[ MODE ] DEFAULT\n\r");
            printf("T_max = %5.2fC T_curr = %5.2fC T_set = %5.2fC PV = %d\n\r", temperature_max, temperature_current, temperature_set, pv_available);
            mode_default::run(temperature_max, temperature_current, temperature_set, pv_available);
        break;
        
        case MODE_BOOST:
            printf("[ MODE ] BOOST\n\r");
        break;
        
        case MODE_AWAY:
            printf("[ MODE ] AWAY\n\r");
        break;
        
        case MODE_NO_GRID:
            printf("[ MODE ] NO GRID\n\r");
        break;
    }
}

//test fucntions. Delete for release
void device_tests::set_temperature_max(double temperature) {
    temperature_max = temperature;
}

void device_tests::set_temperature_current(double temperature) {
    temperature_current = temperature;
}

void device_tests::set_temperature_scheduled(double temperature) {
    temperature_set = temperature;
}

void device_tests::set_pv_available(bool available) {
    pv_available = available;
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
