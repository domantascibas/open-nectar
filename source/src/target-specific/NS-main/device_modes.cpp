#include "consts.h"
#include "device_modes.h"
#include "DataService.h"
#include "error_controller.h"
#include "Sanitizer.h"
// #include "BoostTimeout.h"

// Ticker update_mode_tick;
// RelayController relayController;

namespace device_modes {
  // bool reachedMaxTemp = false;
  // bool updateHeaterMode = false;
  // static bool isFirst = true;
  // uint8_t relayStateNew = TURN_OFF_ALL;
  
  void update_mode_ISR() {
  //   updateHeaterMode = true;
  }
  
  bool isGridRelayOn() {
    // return relayController.isGridRelayOn();
    return 1;
  }
  
  bool isSunRelayOn() {
    // return relayController.isSunRelayOn();
    return 1;
  }
	
	bool turnOffRelays() {
		// relayStateNew = TURN_OFF_ALL;
    // relayController.setRelays(relayStateNew);
		// return true;
    return 1;
	}
  
  void setup() {
    // update_mode_tick.attach(update_mode_ISR, 1.5);
    // relayController.init();
  }
  
  void reset() {
    // DataService::resetData();
    // relayStateNew = TURN_OFF_ALL;
    // relayController.setRelays(relayStateNew);
    // isFirst = true;
  }

  void loop() {
//     if(updateHeaterMode && relayController.finishedSwitching() && !DataService::getCalibrate()) {
//       updateHeaterMode = false;
      
//       float temp = temperatureData.getTemperature();
//       float temp_boiler = temperatureData.getBoilerTemperature();
//       float temp_min = temperatureData.getMinTemperature();
//       float temp_max = temperatureData.getMaxTemperature();
      
//       if(reachedMaxTemp && ((temp_boiler + HIST) < temp_max)) reachedMaxTemp = false;
//       if(reachedMaxTemp) {
//         relayStateNew = TURN_OFF_ALL;
//       } else {
//         switch(DataService::getCurrentHeaterMode()) {
//           default:
//           case nectar_contract::None:
//             printf("MODE AUTO\n"); 
// 						if(isFirst) {
//               isFirst = false;
//               if(temp_boiler < temp) {
//                 relayStateNew = TURN_ON_GRID;
//               } else {
//                 relayStateNew = TURN_ON_SUN;
//               }
//             } else {
//               if(relayController.isGridRelayOn()) {
//                 if(temp_boiler < (temp + HIST)) {
//                   relayStateNew = TURN_ON_GRID;
//                 } else {
//                   relayStateNew = TURN_ON_SUN;
//                 }
//               } else if(relayController.isSunRelayOn()) {
//                 if(temp_boiler > (temp - HIST)) {
//                   relayStateNew = TURN_ON_SUN;
//                 } else {
//                   relayStateNew = TURN_ON_GRID;
//                 }
//               } else {
//                 if(temp_boiler < temp) {
//                   relayStateNew = TURN_ON_GRID;
//                 } else {
//                   relayStateNew = TURN_ON_SUN;
//                 }
//               }
//             }
//           break;

//           case nectar_contract::Boost:
//             printf("MODE BOOST\n");
// //						if((temp_boiler > BOOST_TEMP) || boostTimeoutReached()) {
// 						if(temp_boiler > BOOST_TEMP) {
// 							printf("[INFO] Boost finished. Boiler temp %f  > max temp %f\r\n", temp_boiler, BOOST_TEMP);
// //							sanitizerTurnOn(false);
// //							boostTimeoutReset();
// 							DataService::setPreviousHeaterMode();
// 						} else {
// //							boostTimeoutStartCounter();
// 							relayStateNew = TURN_ON_GRID;
// 						}
//           break;

//           case nectar_contract::Away:
//             printf("MODE AWAY\n");
// 						if(relayController.isGridRelayOn()) {
// 							if(temp_boiler < (AWAY_TEMP + HIST)) {
// 								relayStateNew = TURN_ON_GRID;
// 							} else {
// 								relayStateNew = TURN_ON_SUN;
// 							}
// 						} else if(relayController.isSunRelayOn()) {
// 							if(temp_boiler > (AWAY_TEMP - HIST)) {
// 								relayStateNew = TURN_ON_SUN;
// 							} else {
// 								relayStateNew = TURN_ON_GRID;
// 							}
// 						} else {
// 							relayStateNew = TURN_ON_SUN;
// 						}
//           break;

//           case nectar_contract::Nogrid:
//             printf("MODE NO_GRID\n");
// 						relayStateNew = TURN_ON_SUN;
//           break;
					
// 					case nectar_contract::Alloff:
// 						printf("MODE ALL_OFF\n");
// 						relayStateNew = TURN_OFF_ALL;
// 					break;
//         }
//       }
			
// 			if(mainBoardError.has_error(NO_BOILER_TEMP)
// 				|| mainBoardError.has_error(DEVICE_OVERHEAT)
// 				|| mainBoardError.has_error(MIN_TEMPERATURE)
// 				|| mainBoardError.has_error(MAX_TEMPERATURE)
// 			)	{
// 				relayStateNew = TURN_OFF_ALL;
// 				printf("[ERROR] overheat/temperature-related errors\r\n");
// 			}
			
// 			if((relayStateNew == TURN_ON_SUN) && powerBoardError.has_errors) {
// 				relayStateNew = TURN_OFF_ALL;
// 				printf("[ERROR] power board error\r\n");
// 			}
			
// 			if((temp_boiler < temp_min) || (temp_boiler > temp_max)) {
// 				if(temp_boiler > temp_max) {
// 					printf("[INFO] temp %f  > max temp %f\r\n", temp_boiler, temp_max);
// 					reachedMaxTemp = true;
// 				}
// 				relayStateNew = TURN_OFF_ALL;
// 				//data.error = BOILER_TEMP_SENSOR_ERROR;
// 			}
			
// 			if((deviceOpMode.getCurrentMode() == TEST_MODE) && !DataService::isModeSelected()) {
// 				relayStateNew = TURN_OFF_ALL;
// 			}
			
//       if(relayStateNew != relayController.getRelayState()) {
//         relayController.setRelays(relayStateNew);
//       }
//     }
  }
}
