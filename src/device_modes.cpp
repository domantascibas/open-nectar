#include "mbed.h"
#include "device_modes.h"
#include "power_board.h"
#include "stat_counter.h"
#include "OperationalMode.h"
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
  
  void delay(float seconds) {
    delay_t.reset();
    while(delay_t.read() < seconds) {}
  }
  
  void grid_relay_state(bool state) {
    delay_t.reset();
    while(!zero_cross) {}
    while(delay_t.read_us() < 600) {}
    relay_grid = state;
  }
  
  void update_mode_ISR() {
    update_mode = true;
  }
  
  uint8_t response = TURN_OFF_ALL;

  void set_relays() {
    switch(data.relay_state) {
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
        if((!data.generator_on) && (data.power_board_error == 0)) {
          delay(1.0);
          relay_sun = true;
          delay(1.0);
          power_board::start();
          printf("power board start\r\n");
        }
        printf("turn on sun\r\n");
      break;
    }
//      __disable_irq();
    data.grid_relay_on = relay_grid;
    data.sun_relay_on = relay_sun;
//      __enable_irq();
      
  }
  
  void setup() {
    update_mode_tick.attach(update_mode_ISR, 1.0);
    stat_counter::setup();
    delay_t.start();
  }

  void loop() {
    if(update_mode) {
      update_mode = false;
      
      stat_counter::increase();
      
      __disable_irq();
      float *temp = deviceOpMode.getTemperature();
      float temp_boiler = data.temp_boiler;
      float temp_min = data.temp_min;
      float temp_max = data.temp_max;
      float temp_away = data.temp_away;
      __enable_irq();
      
      switch(data.current_mode) {
        default:  //mode is any other value than the ones below
        case None:
          printf("[MODE] DEFAULT\r\n");
          //printf("T_max = %5.2fC T_curr = %5.2fC T_set = %5.2fC PV = %d ", nectar_data.temperature_max, nectar_data.temperature_moment, nectar_data.temperature_scheduled, nectar_data.pv_available);
          if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
            printf("temp %f  > max temp %f\r\n", temp_boiler, temp_max);
            response = TURN_OFF_ALL;
            //data.error = BOILER_TEMP_SENSOR_ERROR;
          } else {
            response = TURN_ON_SUN;
            if(relay_grid) {
              if(temp_boiler < (*temp + HIST)) {
                response = TURN_ON_GRID;
              } else {
                response = TURN_ON_SUN;
              }
            }
            if(relay_sun) {
              if(temp_boiler > (*temp - HIST)) {
                response = TURN_ON_SUN;
              } else {
                response = TURN_ON_GRID;
              }
            }
          }
        break;

        case Boost:
          //TODO after boost completes, return to previous mode
          //TODO boost mode heats water up to 66C
          printf("[MODE] BOOST\r\n");
          if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
            response = TURN_OFF_ALL;
            //data.error = BOILER_TEMP_SENSOR_ERROR;
            if(temp_boiler > temp_max) {
              printf("Boost finished. Boiler temp %f  > max temp %f\r\n", temp_boiler, temp_max);
              //return to default mode after reaching temp_max
              data.current_mode = data.previous_mode;
            }
          } else {
            response = TURN_ON_GRID;
          }
        break;

        case Away:
          printf("[MODE] AWAY\r\n");
          //TODO add temp_max 45.0
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

        case Nogrid:
          printf("[MODE] NO GRID\r\n");
          //TODO turn on grid when temperature drops below 5.0C
          if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
            response = TURN_OFF_ALL;
            //data.error = BOILER_TEMP_SENSOR_ERROR;
          } else {
            response = TURN_ON_SUN;
          }
        break;
      }
      if(response != data.relay_state) {
        data.relay_state = response;
        set_relays();
      }
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
