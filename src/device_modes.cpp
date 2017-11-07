#include "device_modes.h"
#include "stat_counter.h"
#include "DataService.h"

static const float BOOST_TEMP = 70.0;
static const float AWAY_TEMP = 10.0;
static const float HIST = 2.0;

Ticker update_mode_tick;
RelayController relayController;

namespace device_modes {
  bool updateHeaterMode = false;
  static bool isFirst = true;
  uint8_t relayStateNew = TURN_OFF_ALL;
  
  void update_mode_ISR() {
    updateHeaterMode = true;
  }
  
  bool isGridRelayOn() {
    return relayController.isGridRelayOn();
  }
  
  bool isSunRelayOn() {
    return relayController.isSunRelayOn();
  }
  
  void setup() {
//    update_mode_tick.attach(update_mode_ISR, 1.5);
    stat_counter::setup();
    relayController.init();
  }
  
  void reset() {
    DataService::resetData();
    relayStateNew = TURN_OFF_ALL;
    relayController.setRelays(relayStateNew);
    isFirst = true;
  }

  void loop() {
    if(updateHeaterMode) {
      updateHeaterMode = false;
      
      stat_counter::increase();
      
      float temp = temperatureData.getTemperature();
      float temp_boiler = temperatureData.getBoilerTemperature();
      float temp_min = temperatureData.getMinTemperature();
      float temp_max = temperatureData.getMaxTemperature();
      
      switch(DataService::getCurrentHeaterMode()) {
        default:  //mode is any other value than the ones below
        case nectar_contract::None:
          printf("[MODE] DEFAULT\r\n");
          //printf("T_max = %5.2fC T_curr = %5.2fC T_set = %5.2fC PV = %d ", nectar_data.temperature_max, nectar_data.temperature_moment, nectar_data.temperature_scheduled, nectar_data.pv_available);
          if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
            printf("temp %f  > max temp %f\r\n", temp_boiler, temp_max);
            relayStateNew = TURN_OFF_ALL;
            //data.error = BOILER_TEMP_SENSOR_ERROR;
          } else if(isFirst) {
            isFirst = false;
            if(temp_boiler < temp) {
              relayStateNew = TURN_ON_GRID;
            } else {
              relayStateNew = TURN_ON_SUN;
            }
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

        case nectar_contract::Boost:
          //TODO after boost completes, return to previous mode
          //TODO boost mode heats water up to 66C
          printf("[MODE] BOOST\r\n");
          if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
            relayStateNew = TURN_OFF_ALL;
            //data.error = BOILER_TEMP_SENSOR_ERROR;
            if(temp_boiler > BOOST_TEMP) {
              printf("Boost finished. Boiler temp %f  > max temp %f\r\n", temp_boiler, BOOST_TEMP);
              DataService::setPreviousHeaterMode();
            }
          } else {
            relayStateNew = TURN_ON_GRID;
          }
        break;

        case nectar_contract::Away:
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

        case nectar_contract::Nogrid:
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
