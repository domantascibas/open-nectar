#include "mbed.h"
#include "mode_default.h"
#include "utils.h"

uint8_t mode_default::run(double temp_max, double temp_moment, double temp_set, bool pv_available) {
    //if Tcurrent < Tmax ? check if PV avail : do nothing
    if(temp_moment < temp_max) {
        if(pv_available) {
            //heat water
            printf("T < T_MAX\n\r");
            return TURN_ON_PV;
        } else {
            //if Tcurrent < Tset ? use GRID : do nothing
            if(temp_moment < temp_set) {
                //turn on grid power
                printf("T < T_SET\n\r");
                return TURN_ON_GRID;
            } else {
                //do nothing
                printf("T > T_SET\n\r");
                return TURN_OFF_ALL;
            }
        }
    } else {
        printf("T >= T_MAX\n\r");
        return TURN_OFF_ALL;
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
