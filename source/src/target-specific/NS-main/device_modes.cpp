#include "consts.h"
#include "pins.h"
#include "device_modes.h"
#include "DataService.h"
#include "error_handler.h"
// #include "Sanitizer.h"
#include "BoostTimeout.h"

Ticker update_mode_tick;
RelayController relayController;

static uint8_t reachedMaxTemp = 0;
static uint8_t updateHeaterMode = 0;
static uint8_t isFirst = 1;
static uint8_t relayStateNew = TURN_OFF_ALL;

void device_modes_update_mode_ISR(void) {
  updateHeaterMode = 1;
}

void device_modes_setHeaterMode(uint8_t i) {
  updateHeaterMode = i;
}

void device_modes_setup(void) {
    update_mode_tick.attach(device_modes_update_mode_ISR, 1.5);
    relayController.init();
}

void device_modes_reset(void) {
  DataService::resetData();
  relayStateNew = TURN_OFF_ALL;
  relayController.setRelays(relayStateNew);
  isFirst = true;
}

void device_modes_loop() {
  if(updateHeaterMode && relayController.finishedSwitching() && !DataService::getCalibrate()) {
  if(updateHeaterMode) {
    updateHeaterMode = 0;
      
    float temp = temperatureData.getTemperature();
    float temp_boiler = temperatureData.getBoilerTemperature();
    float temp_min = temperatureData.getMinTemperature();
    float temp_max = temperatureData.getMaxTemperature();

    if(reachedMaxTemp && ((temp_boiler + HIST) < temp_max)) reachedMaxTemp = 0;
      if(reachedMaxTemp) {
        relayStateNew = TURN_OFF_ALL;
      } else {
        switch(DataService::getCurrentHeaterMode()) {
          default:
          case nectar_contract::None:
            printf("MODE AUTO\n"); 
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
            printf("MODE BOOST\n");
//						if((temp_boiler > BOOST_TEMP) || boostTimeoutReached()) {
						if(temp_boiler > BOOST_TEMP) {
							printf("[INFO] Boost finished. Boiler temp %f  > max temp %f\r\n", temp_boiler, BOOST_TEMP);
//							sanitizerTurnOn(false);
//							boostTimeoutReset();
							DataService::setPreviousHeaterMode();
						} else {
//							boostTimeoutStartCounter();
							relayStateNew = TURN_ON_GRID;
						}
          break;

          case nectar_contract::Away:
            printf("MODE AWAY\n");
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
            printf("MODE NO_GRID\n");
						relayStateNew = TURN_ON_SUN;
          break;
					
					case nectar_contract::Alloff:
						printf("MODE ALL_OFF\n");
						relayStateNew = TURN_OFF_ALL;
					break;
        }
      }
			
			if(error_isSet(NS_NO_BOILER_TEMP)
				|| error_isSet(NS_DEVICE_OVERHEAT)
				|| error_isSet(NS_MIN_TEMPERATURE)
				|| error_isSet(NS_MAX_TEMPERATURE)
			)	{
				relayStateNew = TURN_OFF_ALL;
				printf("[ERROR] overheat/temperature-related errors\r\n");
			}
			
			// if((relayStateNew == TURN_ON_SUN) && powerBoardError.has_errors) {
			if((relayStateNew == TURN_ON_SUN) && error_hasError()) {
				relayStateNew = TURN_OFF_ALL;
				printf("[ERROR] power board error\r\n");
			}
			
			if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
				if(temp_boiler > temp_max) {
					printf("[INFO] temp %f  > max temp %f\r\n", temp_boiler, temp_max);
					reachedMaxTemp = true;
				}
				relayStateNew = TURN_OFF_ALL;
				//data.error = BOILER_TEMP_SENSOR_ERROR;
			}
			
			if((deviceOpMode_getCurrentMode() == TEST_MODE) && !DataService::isModeSelected()) {
				relayStateNew = TURN_OFF_ALL;
			}
			
      if(relayStateNew != relayController.getRelayState()) {
        relayController.setRelays(relayStateNew);
      }
    }
  }
}
