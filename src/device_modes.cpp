#include "mbed.h"
#include "device_modes.h"
#include "power_board.h"
#include "data.h"

static const PinName SUN = PB_3;
static const PinName GRID = PB_4;
static const PinName ZERO_CROSS = PB_5;
static const float HIST = 5.0;

Timer delay_t;
DigitalOut relay_sun(SUN);
DigitalOut relay_grid(GRID);
DigitalIn zero_cross(ZERO_CROSS);

namespace device_modes {
  enum power_source {
    TURN_ON_GRID,
    TURN_ON_PV,
    TURN_OFF_ALL
  };

  enum modes {
    MODE_DEFAULT,
    MODE_BOOST,
    MODE_AWAY,
    MODE_NO_GRID
  };
  
  void delay(float seconds) {
    delay_t.reset();
    delay_t.start();
    while(delay_t.read() < seconds) {}
    delay_t.stop();
  }
  
  void grid_relay_state(bool state) {
    delay_t.reset();
    while(!zero_cross) {}
    delay_t.start();
    while(delay_t.read_us() < 600) {}
    delay_t.stop();
    relay_grid = state;
  }
  
  uint8_t response = TURN_OFF_ALL;

  uint8_t set_relays(uint8_t state) {
    switch(state) {
      default:
      case TURN_OFF_ALL:
        power_board::stop();
        while(data.pv_current > 0.05) {}
        relay_sun = false;
        grid_relay_state(false);
      break;

      case TURN_ON_GRID:
        power_board::stop();
        while(data.pv_current > 0.05) {}
        relay_sun = false;
        delay(1.0);
        grid_relay_state(true);
      break;

      case TURN_ON_PV:
        grid_relay_state(false);
        delay(1.0);
        relay_sun = true;
        delay(1.0);
        power_board::start();
      break;
    }
    data.grid_relay_on = relay_grid;
    data.sun_relay_on = relay_sun;
    return 0;   //TODO change to meaningful return
  }

  void loop(void) {
    switch(data.current_mode) {
      default:  //mode is any other value than the ones below
      case MODE_DEFAULT:
        //printf("[MODE] DEFAULT\r\n");
        //printf("T_max = %5.2fC T_curr = %5.2fC T_set = %5.2fC PV = %d ", nectar_data.temperature_max, nectar_data.temperature_moment, nectar_data.temperature_scheduled, nectar_data.pv_available);
        if((data.temp_boiler < data.temp_min) || (data.temp_boiler > data.temp_max)) {
          response = TURN_OFF_ALL;
          //data.error = BOILER_TEMP_SENSOR_ERROR;
        } else {
          response = TURN_ON_PV;
          if(data.grid_relay_on) {
            if(data.temp_boiler < (data.temp_scheduled + HIST)) {
              response = TURN_ON_GRID;
            } else {
              response = TURN_ON_PV;
            }
          }
          if(data.sun_relay_on) {
            if(data.temp_boiler > (data.temp_scheduled - HIST)) {
              response = TURN_ON_PV;
            } else {
              response = TURN_ON_GRID;
            }
          }
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
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
