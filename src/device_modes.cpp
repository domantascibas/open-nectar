#include "device_modes.h"
#include "stat_counter.h"
#include "DataService.h"

static const float BOOST_TEMP = 70.0;
static const float AWAY_TEMP = 10.0;
static const float HIST = 2.0;

Ticker update_mode_tick;
RelayController relayController;

namespace device_modes {
  bool reachedMaxTemp = false;
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
      
      float temp = temperatureData.getTemperature();
      float temp_boiler = temperatureData.getBoilerTemperature();
      float temp_min = temperatureData.getMinTemperature();
      float temp_max = temperatureData.getMaxTemperature();
      
      if(reachedMaxTemp && ((temp_boiler - 5.0) < temp_max)) reachedMaxTemp = false;
      if(reachedMaxTemp) {
        relayStateNew = TURN_OFF_ALL;
      } else {
        switch(DataService::getCurrentHeaterMode()) {
          default:
          case nectar_contract::None:
            printf("[MODE] DEFAULT\r\n");
            if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
              if(temp_boiler > temp_max) {
                printf("temp %f  > max temp %f\r\n", temp_boiler, temp_max);
                reachedMaxTemp = true;
              }
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
            printf("[MODE] BOOST\r\n");
            if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
              if(temp_boiler > temp_max) {
                printf("temp %f  > max temp %f\r\n", temp_boiler, temp_max);
                reachedMaxTemp = true;
              }
              relayStateNew = TURN_OFF_ALL;
              //data.error = BOILER_TEMP_SENSOR_ERROR;
            } else {
              if(temp_boiler > BOOST_TEMP) {
                printf("Boost finished. Boiler temp %f  > max temp %f\r\n", temp_boiler, BOOST_TEMP);
                DataService::setPreviousHeaterMode();
              } else {
                relayStateNew = TURN_ON_GRID;
              }
            }
          break;

          case nectar_contract::Away:
            printf("[MODE] AWAY\r\n");
            if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
              if(temp_boiler > temp_max) {
                printf("temp %f  > max temp %f\r\n", temp_boiler, temp_max);
                reachedMaxTemp = true;
              }
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
            if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
              if(temp_boiler > temp_max) {
                printf("temp %f  > max temp %f\r\n", temp_boiler, temp_max);
                reachedMaxTemp = true;
              }
              relayStateNew = TURN_OFF_ALL;
              //data.error = BOILER_TEMP_SENSOR_ERROR;
            } else {
              relayStateNew = TURN_ON_SUN;
              printf("[NO GRID] TURN ON SUN\r\n");
            }
          break;
        }
      }
      
      if(relayStateNew != relayController.getRelayState()) {
        relayController.setRelays(relayStateNew);
      }
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
