#include "mbed.h"
#include "modes.h"
#include "utils.h"

DigitalOut relay_sun_on(PB_3);
DigitalOut relay_grid_on(PB_4);

//currently defaults. Must initialize them from ESP
uint8_t mode_current = MODE_DEFAULT;
uint8_t response = TURN_OFF_ALL;
bool pv_available = false;

double temperature_max;
double temperature_set;
double temperature_moment;

void device::set_mode(uint8_t mode_new) {
    mode_current = mode_new;
}

uint8_t device::set_relays(uint8_t state) {
    switch(state) {
        default:
        case TURN_OFF_ALL:
            relay_sun_on = false;
            relay_grid_on = false;
            printf("ALL OFF\n\r");
        break;
        
        case TURN_ON_GRID:
            relay_sun_on = false;
            relay_grid_on = true;
            printf("GRID ON\n\r");
        break;
        
        case TURN_ON_PV:
            relay_grid_on = false;
            relay_sun_on = true;
            printf("PV ON\n\r");
        break;
    }
    printf("\n\r");
    return 0;   //change to meaningful return
}

uint8_t device::get_mode() {
    return mode_current;
}

void device::run(uint8_t mode) {
    switch(mode) {
        default:  //mode is any other value than the ones below
        case MODE_DEFAULT:
            printf("[ MODE ] DEFAULT\n\r");
            printf("T_max = %5.2fC T_curr = %5.2fC T_set = %5.2fC PV = %d\n\r", temperature_max, temperature_moment, temperature_set, pv_available);
            //if Tcurrent < Tmax ? check if PV avail : do nothing
            if(temperature_moment < temperature_max) {
                if(pv_available) {
                    //heat water
                    printf("T < T_MAX\n\r");
                    response = TURN_ON_PV;
                } else {
                    //if Tcurrent < Tset ? use GRID : do nothing
                    if(temperature_moment < temperature_set) {
                        //turn on grid power
                        printf("T < T_SET\n\r");
                        response = TURN_ON_GRID;
                    } else {
                        //do nothing
                        printf("T > T_SET\n\r");
                        response = TURN_OFF_ALL;
                    }
                }
            } else {
                printf("T >= T_MAX\n\r");
                response = TURN_OFF_ALL;
            }
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
    set_relays(response);
}

//test functions. Delete for release
void device_tests::set_temperature_max(double temperature) {
    temperature_max = temperature;
}

void device_tests::set_temperature_current(double temperature) {
    temperature_moment = temperature;
}

void device_tests::set_temperature_scheduled(double temperature) {
    temperature_set = temperature;
}

void device_tests::set_pv_available(bool available) {
    pv_available = available;
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
