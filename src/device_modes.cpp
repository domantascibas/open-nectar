#include "device_modes.h"
#include "DataService.h"
#include "ErrorHandler.h"
#include "Sanitizer.h"
#include "BoostTimeout.h"

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
	
	bool turnOffRelays() {
		relayStateNew = TURN_OFF_ALL;
    relayController.setRelays(relayStateNew);
		return true;
	}
  
  void setup() {
    update_mode_tick.attach(update_mode_ISR, 1.5);
    relayController.init();
  }
  
  void reset() {
    DataService::resetData();
    relayStateNew = TURN_OFF_ALL;
    relayController.setRelays(relayStateNew);
    isFirst = true;
  }

  void loop() {
    if(updateHeaterMode && relayController.finishedSwitching() && !DataService::getCalibrate()) {
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
						if(isFirst) {
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
                if(temp_boiler < temp) {
                  relayStateNew = TURN_ON_GRID;
                } else {
                  relayStateNew = TURN_ON_SUN;
                }
              }
            }
          break;

          case nectar_contract::Boost:
            printf("[MODE] BOOST\r\n");
//						if((temp_boiler > BOOST_TEMP) || boostTimeoutReached()) {
						if(temp_boiler > BOOST_TEMP) {
							printf("Boost finished. Boiler temp %f  > max temp %f\r\n", temp_boiler, BOOST_TEMP);
//							sanitizerTurnOn(false);
//							boostTimeoutReset();
							DataService::setPreviousHeaterMode();
						} else {
//							boostTimeoutStartCounter();
							relayStateNew = TURN_ON_GRID;
						}
          break;

          case nectar_contract::Away:
            printf("[MODE] AWAY\r\n");
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
          break;

          case nectar_contract::Nogrid:
            printf("[MODE] NO GRID\r\n");
						relayStateNew = TURN_ON_SUN;
          break;
        }
      }
			
			if(powerData.sun_voltage < 10.0){
				if(deviceOpMode.isConfigured()) {
					if(((relayController.getRelayState() == TURN_ON_SUN) && (relayStateNew == TURN_ON_SUN)) || ((relayController.getRelayState() == TURN_OFF_SUN) && (relayStateNew == TURN_ON_SUN)))
						relayStateNew = TURN_OFF_SUN;
				} else {
					if(!DataService::isDayTime()) {
						if(((relayController.getRelayState() == TURN_ON_SUN) && (relayStateNew == TURN_ON_SUN)) || ((relayController.getRelayState() == TURN_OFF_SUN) && (relayStateNew == TURN_ON_SUN)))
							relayStateNew = TURN_OFF_SUN;
					}
				}
			}
			
			if(mainBoardError.has_error(NO_BOILER_TEMP)
				|| mainBoardError.has_error(DEVICE_OVERHEAT)
				|| mainBoardError.has_error(MIN_TEMPERATURE)
				|| mainBoardError.has_error(MAX_TEMPERATURE)
			)	{
				relayStateNew = TURN_OFF_ALL;
				printf("overheat/temperature-related errors\r\n");
			}
			
			if((relayStateNew == TURN_ON_SUN) && powerBoardError.has_errors) {
				relayStateNew = TURN_OFF_ALL;
				printf("power board error\r\n");
			}
			
			if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
				if(temp_boiler > temp_max) {
					printf("temp %f  > max temp %f\r\n", temp_boiler, temp_max);
					reachedMaxTemp = true;
				}
				relayStateNew = TURN_OFF_ALL;
				//data.error = BOILER_TEMP_SENSOR_ERROR;
			}
			
      if(relayStateNew != relayController.getRelayState()) {
        relayController.setRelays(relayStateNew);
      }
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
