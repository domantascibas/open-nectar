#include "mbed.h"
#include "modes.h"
#include "data.h"

Timer delay_t;
DigitalOut relay_sun(PB_3);
DigitalOut relay_grid(PB_4);
DigitalIn zero_cross(PB_5);
extern Data data;

namespace nectar {
    uint8_t response = TURN_OFF_ALL;
    
    uint8_t set_relays(uint8_t state) {
        switch(state) {
            default:
            case TURN_OFF_ALL:
                relay_sun = false;
                while(!zero_cross) {}
                delay_t.start();
                while(delay_t.read_us() < 600) {}
                delay_t.stop();
                relay_grid = false;
                //printf("ALL OFF\r\n");
            break;
            
            case TURN_ON_GRID:
                relay_sun = false;
                delay_t.start();
                while(delay_t.read_ms() < 1000) {}
                delay_t.stop();
                delay_t.reset();
                while(!zero_cross) {}
                delay_t.start();
                while(delay_t.read_us() < 600) {}
                delay_t.stop();
                relay_grid = true;
                //printf("GRID ON\r\n");
            break;
            
            case TURN_ON_PV:
                while(!zero_cross) {}
                delay_t.start();
                while(delay_t.read_us() < 600) {}
                delay_t.stop();
                delay_t.reset();
                relay_grid = false;
                delay_t.start();
                while(delay_t.read_ms() < 1000) {}
                delay_t.stop();
                delay_t.reset();
                relay_sun = true;
                //printf("PV ON\r\n");
            break;
        }
        //printf("\n\r");
        data.grid_relay_on = relay_grid;
        data.sun_relay_on = relay_sun;
        return 0;   //change to meaningful return
    }

    uint8_t loop(void) {
        switch(data.current_mode) {
            default:  //mode is any other value than the ones below
            case MODE_DEFAULT:
                //printf("[MODE] DEFAULT\r\n");
                //printf("T_max = %5.2fC T_curr = %5.2fC T_set = %5.2fC PV = %d ", nectar_data.temperature_max, nectar_data.temperature_moment, nectar_data.temperature_scheduled, nectar_data.pv_available);
                //Tcurrent < Tmax ? check if PV avail : do nothing
                if(data.temp_boiler < data.temp_max) {
                    response = TURN_ON_PV;
                    if(data.temp_boiler < data.temp_scheduled) {
                        response = TURN_ON_GRID;
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
