#include "mbed.h"
#include "EspComms.h"
#include "PowerBoardComms.h"
#include "device_modes.h"
#include "service.h"
#include "menu_service.h"
#include "OperationalMode.h"
#include "ErrorHandler.h"
#include "TemperatureController.h"

TemperatureController tempController;
bool inErrorScreen = false;

int main() {
  static bool isFirst = true;
  mainBoardError.save_error_code(0x300);
  service::setup();
  menu_service::setup();
  tempController.init();
  wait(3.0);
  tempController.updateTemperatures(&data.temp_boiler, &data.device_temperature);
  power_board::setup();
  esp::setup();
  device_modes::setup();
  
  menu_service::needUpdate = true;

  while(1) {
    if((mainBoardError.has_errors || powerBoardError.has_errors) && !inErrorScreen) {
      menu_service::needUpdate = true;
      inErrorScreen = true;
    } else {
      inErrorScreen = false;
    }
    
    if(menu_service::needUpdate) {
      menu_service::updateScreen();
//      printf("Available memory = %d\r\n\n", Memory::availableMemory(1) );
    }
    
    switch(deviceOpMode.currentMode) {
      default:
      case NOT_CONFIGURED:
        if(esp::espData.is_configured) deviceOpMode.setConfigured();
        device_modes::loop();
        power_board::loop();
        break;
      
      case CONFIGURED:
        device_modes::loop();
        power_board::loop();
        break;
      
      case WELCOME:
        //nothing to do here
        //wait for user to finish onboarding
        break;
      
      case TEST_MODE:
        if(isFirst) {
          isFirst = false;
          power_board::enter_test_mode();
        }
        device_modes::loop();
        power_board::loop();
        break;
    }
    
    tempController.updateTemperatures(&data.temp_boiler, &data.device_temperature);
    __WFI();
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
