#include "mbed.h"
#include "mode_default.h"
#include "utils.h"

uint8_t mode_default::run(double temp_max, double temp_current, double temp_set, bool pv_available) {
    //if Tcurrent < Tmax ? check if PV avail : do nothing
    if(temp_current < temp_max) {
        if(pv_available) {
            //heat water
            printf("PV ON: T < T_MAX\n\r");
            return TURN_ON_PV;
        } else {
            //if Tcurrent < Tset ? use GRID : do nothing
            if(temp_current < temp_set) {
                //turn on grid power
                printf("GRID ON: T < T_SET\n\r");
                return TURN_ON_GRID;
            } else {
                //do nothing
                printf("ALL OFF: T > T_SET\n\r");
                return TURN_OFF_ALL;
            }
        }
    } else {
        printf("ALL OFF: T >= T_MAX\n\r");
        return TURN_OFF_ALL;
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
