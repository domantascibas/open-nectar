#include "mbed.h"
#include "modes.h"
#include "mode_default.h"
#include "utils.h"

//currently defaults. Must initialize them from ESP
int mode_current = MODE_DEFAULT;
double temperature_max = 80.0;
double temperature_current = 55.0;
bool pv_available = true;

void device::set_mode(int mode_new) {
    mode_current = mode_new;
}

int device::get_mode() {
    return mode_current;
}

void device::run(int mode) {
    switch(mode) {
        default:  //mode is any other value than the ones below
        case MODE_DEFAULT:
            printf("[ MODE ] DEFAULT\n\r");
            mode_default::run(temperature_max, temperature_current, pv_available);
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
    temperature_current = temperature;
}

void device_tests::set_temperature_current(double temperature) {
    temperature_max = temperature;
}

void device_tests::set_pv_available(bool available) {
    pv_available = available;
}

// *******************************Nectar Sun Copyright � Nectar Sun 2017*************************************   
