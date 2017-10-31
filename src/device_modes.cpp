#include "device_modes.h"
#include "stat_counter.h"
#include "OperationalMode.h"
#include "data.h"
#include "EspComms.h"

static const float BOOST_TEMP = 70.0;
static const float AWAY_TEMP = 10.0;
static const float HIST = 2.0;

Ticker update_mode_tick;
RelayController relayController;

namespace device_modes {
  volatile bool update_mode = false;
  uint8_t relayStateNew = TURN_OFF_ALL;
  
  void update_mode_ISR() {
    update_mode = true;
  }
  
  void setup() {
    update_mode_tick.attach(update_mode_ISR, 1.5);
    stat_counter::setup();
    relayController.init();
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
      __enable_irq();

      switch(deviceOpMode.getHeaterMode()) {
        default:  //mode is any other value than the ones below
        case None:
          printf("[MODE] DEFAULT\r\n");
          //printf("T_max = %5.2fC T_curr = %5.2fC T_set = %5.2fC PV = %d ", nectar_data.temperature_max, nectar_data.temperature_moment, nectar_data.temperature_scheduled, nectar_data.pv_available);
          if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
            printf("temp %f  > max temp %f\r\n", temp_boiler, temp_max);
            relayStateNew = TURN_OFF_ALL;
            //data.error = BOILER_TEMP_SENSOR_ERROR;
          } else {
            if(relayController.isGridRelayOn()) {
              if(temp_boiler < (temp + HIST)) {
                relayStateNew = TURN_ON_GRID;
              } else {
                relayStateNew = TURN_ON_SUN;
              }
            } else if(relayController.isSunRelayOn()) {
              if(temp_boiler > (temp - HIST)) {
                relayStateNew = TURN_ON_SUN;
              } else {
                relayStateNew = TURN_ON_GRID;
              }
            } else {
              relayStateNew = TURN_ON_SUN;
            }
          }
        break;

        case Boost:
          //TODO after boost completes, return to previous mode
          //TODO boost mode heats water up to 66C
          printf("[MODE] BOOST\r\n");
          if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
            relayStateNew = TURN_OFF_ALL;
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
            relayStateNew = TURN_ON_GRID;
          }
        break;

        case Away:
          printf("[MODE] AWAY\r\n");
          //TODO add temp_max 45.0
          if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
            relayStateNew = TURN_OFF_ALL;
            //data.error = BOILER_TEMP_SENSOR_ERROR;
          } else {
            if(relayController.isGridRelayOn()) {
              if(temp_boiler < (AWAY_TEMP + HIST)) {
                relayStateNew = TURN_ON_GRID;
              } else {
                relayStateNew = TURN_ON_SUN;
              }
            } else if(relayController.isSunRelayOn()) {
              if(temp_boiler > (AWAY_TEMP - HIST)) {
                relayStateNew = TURN_ON_SUN;
              } else {
                relayStateNew = TURN_ON_GRID;
              }
            } else {
              relayStateNew = TURN_ON_SUN;
            }
          }
        break;

        case Nogrid:
          printf("[MODE] NO GRID\r\n");
          //TODO turn on grid when temperature drops below 5.0C
          if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
            relayStateNew = TURN_OFF_ALL;
            printf("[NO GRID] TURN OFF ALL\r\n");
            printf("[NO GRID] min:%f < %f < max:%f\r\n", temp_min, temp_boiler, temp_max);
            //data.error = BOILER_TEMP_SENSOR_ERROR;
          } else {
            relayStateNew = TURN_ON_SUN;
            printf("[NO GRID] TURN ON SUN\r\n");
          }
        break;
      }
      
      if(relayStateNew != relayController.getRelayState()) {
        relayController.setRelays(relayStateNew);
      }
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
