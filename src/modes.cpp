#include "mbed.h"
#include "modes.h"
#include "data.h"

DigitalOut relay_sun_on(PB_3);
DigitalOut relay_grid_on(PB_4);

extern Data data;

namespace nectar {
    uint8_t response = TURN_OFF_ALL;
    
    uint8_t set_relays(uint8_t state) {
        switch(state) {
            default:
            case TURN_OFF_ALL:
                relay_sun_on = false;
                relay_grid_on = false;
                //printf("ALL OFF\r");
            break;
            
            case TURN_ON_GRID:
                relay_sun_on = false;
                wait(1);
                relay_grid_on = true;
                //printf("GRID ON\r");
            break;
            
            case TURN_ON_PV:
                relay_grid_on = false;
                wait(1);
                relay_sun_on = true;
                //printf("PV ON\r");
            break;
        }
        //printf("\n\r");
        return 0;   //change to meaningful return
    }

    uint8_t loop(void) {
        switch(data.current_mode) {
            default:  //mode is any other value than the ones below
            case MODE_DEFAULT:
                //printf("[ MODE ] DEFAULT ");
                //printf("T_max = %5.2fC T_curr = %5.2fC T_set = %5.2fC PV = %d ", nectar_data.temperature_max, nectar_data.temperature_moment, nectar_data.temperature_scheduled, nectar_data.pv_available);
                //Tcurrent < Tmax ? check if PV avail : do nothing
                if(data.temp_boiler < data.temp_max) {
                    if(data.pv_available) {
                        //heat water
                        //printf("T < T_MAX ");
                        response = TURN_ON_PV;
                    } else {
                        //if Tcurrent < Tset ? use GRID : do nothing
                        if(data.temp_boiler < data.temp_scheduled) {
                            //turn on grid power
                            //printf("T < T_SET ");
                            response = TURN_ON_GRID;
                        } else {
                            //do nothing
                            //printf("T > T_SET ");
                            response = TURN_OFF_ALL;
                        }
                    }
                } else {
                    //printf("T >= T_MAX ");
                    response = TURN_OFF_ALL;
                }
            break;
            
            case MODE_BOOST:
                //printf("[ MODE ] BOOST\n\r");
                //Tcurrent < Tmax ? use GRID : do nothing
                if(data.temp_boiler < data.temp_max) {
                    response = TURN_ON_GRID;
                } else {
                    response = TURN_OFF_ALL;
                }
            break;
            
            case MODE_AWAY:
                //printf("[ MODE ] AWAY\n\r");
                //pv available ? use PV : check Tcurrent < Tmax
                if(data.pv_available) {
                    response = TURN_ON_PV;
                } else {
                    //Tcurrent > Tmin ? do nothing : use GRID
                    if(data.temp_boiler > data.temp_min) {
                        response = TURN_OFF_ALL;
                    } else {
                        response = TURN_ON_GRID;
                    }
                }
            break;
            
            case MODE_NO_GRID:
                //printf("[ MODE ] NO GRID\n\r");
                if(data.pv_available) {
                    response = TURN_ON_PV;
                } else {
                    response = TURN_OFF_ALL;
                }
            break;
        }
        set_relays(response);
        return response;
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
