#include "device_modes.h"
#include "stat_counter.h"
#include "OperationalMode.h"
#include "RelayController.h"
#include "data.h"
#include "EspComms.h"

static const float BOOST_TEMP = 70.0;
static const float HIST = 2.0;

Ticker update_mode_tick;
RelayController relayController;

namespace device_modes {
  volatile bool update_mode = false;
  
  void update_mode_ISR() {
    update_mode = true;
  }
  
  void setup() {
    update_mode_tick.attach(update_mode_ISR, 1.0);
    stat_counter::setup();
    relayController.turnOffAll();
  }

  void loop() {
    if(update_mode) {
      update_mode = false;
      
      stat_counter::increase();
      
      __disable_irq();
      float temp = deviceOpMode.getTemperature();
      float temp_boiler = data.temp_boiler;
      float temp_min = data.temp_min;
      float temp_max = deviceOpMode.getTemperatureMax();
      float temp_away = data.temp_away;
      __enable_irq();

      switch(deviceOpMode.getHeaterMode()) {
        default:  //mode is any other value than the ones below
        case None:
          printf("[MODE] DEFAULT\r\n");
          //printf("T_max = %5.2fC T_curr = %5.2fC T_set = %5.2fC PV = %d ", nectar_data.temperature_max, nectar_data.temperature_moment, nectar_data.temperature_scheduled, nectar_data.pv_available);
          if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
            printf("temp %f  > max temp %f\r\n", temp_boiler, temp_max);
            relayController.turnOffAll();
            //data.error = BOILER_TEMP_SENSOR_ERROR;
          } else {
            if(relayController.isGridRelayOn()) {
              if(temp_boiler < (temp + HIST)) {
                relayController.turnOnGrid();
              } else {
                relayController.turnOnSun();
              }
            } else if(relayController.isSunRelayOn()) {
              if(temp_boiler > (temp - HIST)) {
                relayController.turnOnSun();
              } else {
                relayController.turnOnGrid();
              }
            } else {
              relayController.turnOnSun();
            }
          }
        break;

        case Boost:
          //TODO after boost completes, return to previous mode
          //TODO boost mode heats water up to 66C
          printf("[MODE] BOOST\r\n");
          if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
            relayController.turnOffAll();
            //data.error = BOILER_TEMP_SENSOR_ERROR;
            if(temp_boiler > BOOST_TEMP) {
              printf("Boost finished. Boiler temp %f  > max temp %f\r\n", temp_boiler, BOOST_TEMP);
              //return to default mode after reaching temp_max in NO_CONFIG
              if(deviceOpMode.getCurrentMode() != CONFIGURED) {
                data.current_mode = data.previous_mode;
              } else {
                data.boost_off = true;
                data.current_mode = (nectar_contract::HeaterMode)esp::espData.heater_mode;
                esp::get_data_ISR();
              }
            }
          } else {
            relayController.turnOnGrid();
          }
        break;

        case Away:
          printf("[MODE] AWAY\r\n");
          //TODO add temp_max 45.0
          if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
            relayController.turnOffAll();
            //data.error = BOILER_TEMP_SENSOR_ERROR;
          } else {
            if(relayController.isGridRelayOn()) {
              if(temp_boiler < (temp_away + HIST)) {
                relayController.turnOnGrid();
              } else {
                relayController.turnOnSun();
              }
            } else if(relayController.isSunRelayOn()) {
              if(temp_boiler > (temp_away - HIST)) {
                relayController.turnOnSun();
              } else {
                relayController.turnOnGrid();
              }
            } else {
              relayController.turnOnSun();
            }
          }
        break;

        case Nogrid:
          printf("[MODE] NO GRID\r\n");
          //TODO turn on grid when temperature drops below 5.0C
          if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
            relayController.turnOffAll();
            //data.error = BOILER_TEMP_SENSOR_ERROR;
          } else {
            relayController.turnOnSun();
          }
        break;
      }
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
