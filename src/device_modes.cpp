#include "mbed.h"
#include "device_modes.h"
#include "power_board.h"
#include "stat_counter.h"
#include "data.h"

static const PinName SUN = PB_3;
static const PinName GRID = PB_4;
static const PinName ZERO_CROSS = PB_5;
static const float HIST = 2.0;

Ticker update_mode_tick;
Timer delay_t;
DigitalOut relay_sun(SUN);
DigitalOut relay_grid(GRID);
DigitalIn zero_cross(ZERO_CROSS);

namespace device_modes {
  volatile bool update_mode = false;
  
  enum power_source {
    TURN_ON_GRID,
    TURN_ON_SUN,
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
  
  void update_mode_ISR() {
    update_mode = true;
  }
  
  uint8_t response = TURN_OFF_ALL;

  uint8_t set_relays(uint8_t state) {
    static uint8_t last_state;
//    if(state != last_state) {
      switch(state) {
        default:
        case TURN_OFF_ALL:
          power_board::stop();
          //while(data.pv_current > 0.1) {}
          delay(1.0);
          relay_sun = false;
          grid_relay_state(false);
          printf("turn off all\r\n");
        break;

        case TURN_ON_GRID:
          power_board::stop();
          delay(1.0);
          //while(data.pv_current > 0.1) {}
          relay_sun = false;
          delay(1.0);
          grid_relay_state(true);
          printf("turn on grid\r\n");
        break;

        case TURN_ON_SUN:
          grid_relay_state(false);
          if((!data.generator_on) && (data.error == 0)) {
            delay(1.0);
            relay_sun = true;
            delay(1.0);
            power_board::start();
            printf("power board start\r\n");
          }
          printf("turn on sun\r\n");
        break;
      }
      __disable_irq();
      data.grid_relay_on = relay_grid;
      data.sun_relay_on = relay_sun;
      __enable_irq();
//    }
//    last_state = state;
    return 0;   //TODO change to meaningful return
  }
  
  void setup() {
    update_mode_tick.attach(update_mode_ISR, 1.0);
    stat_counter::setup();
  }

  void loop() {
    if(update_mode && !line_busy) {
      update_mode = false;
      
      stat_counter::increase();
      
      __disable_irq();
      float temp_boiler = data.temp_boiler;
      float temp_min = data.temp_min;
      float temp_max = data.temp_max;
      float temp_scheduled = data.temp_scheduled;
      float temp_away = data.temp_away;
      __enable_irq();
      
      switch(data.current_mode) {
        default:  //mode is any other value than the ones below
        case MODE_DEFAULT:
          printf("[MODE] DEFAULT\r\n");
          //printf("T_max = %5.2fC T_curr = %5.2fC T_set = %5.2fC PV = %d ", nectar_data.temperature_max, nectar_data.temperature_moment, nectar_data.temperature_scheduled, nectar_data.pv_available);
          if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
            response = TURN_OFF_ALL;
            //data.error = BOILER_TEMP_SENSOR_ERROR;
          } else {
            response = TURN_ON_SUN;
            if(relay_grid) {
              if(temp_boiler < (temp_scheduled + HIST)) {
                response = TURN_ON_GRID;
              } else {
                response = TURN_ON_SUN;
              }
            }
            if(relay_sun) {
              if(temp_boiler > (temp_scheduled - HIST)) {
                response = TURN_ON_SUN;
              } else {
                response = TURN_ON_GRID;
              }
            }
          }
        break;

        case MODE_BOOST:
          printf("[MODE] BOOST\r\n");
          if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
            response = TURN_OFF_ALL;
            //data.error = BOILER_TEMP_SENSOR_ERROR;
            if(temp_boiler > temp_max) {
              //return to default mode after reaching temp_max
              data.current_mode = MODE_DEFAULT;
            }
          } else {
            response = TURN_ON_GRID;
          }
        break;

        case MODE_AWAY:
          if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
            response = TURN_OFF_ALL;
            //data.error = BOILER_TEMP_SENSOR_ERROR;
          } else {
            response = TURN_ON_SUN;
            if(relay_grid) {
              if(temp_boiler < (temp_away + HIST)) {
                response = TURN_ON_GRID;
              } else {
                response = TURN_ON_SUN;
              }
            }
            if(relay_sun) {
              if(temp_boiler > (temp_away - HIST)) {
                response = TURN_ON_SUN;
              } else {
                response = TURN_ON_GRID;
              }
            }
          }
        break;

        case MODE_NO_GRID:
          if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
            response = TURN_OFF_ALL;
            //data.error = BOILER_TEMP_SENSOR_ERROR;
          } else {
            response = TURN_ON_SUN;
          }
        break;
      }
      set_relays(response);
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
