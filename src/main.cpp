#include "mbed.h"
#include "ServiceComms.h"
#include "EspComms.h"
#include "PowerBoardComms.h"
#include "device_modes.h"
#include "menu_service.h"
#include "ErrorHandler.h"
#include "TemperatureController.h"
#include "DataService.h"

bool inErrorScreen = false;

TemperatureController tempController;

int main() {
  static bool isFirst = true;
  service::setup();
  
  menu_service::setup();
  wait(1.0);
  tempController.init();
  wait(3.0);
  tempController.updateTemperatures();
  power_board::setup();
  esp::setup();
  device_modes::setup();
  
  menu_service::needUpdate = true;

  while(1) {
    if((mainBoardError.has_errors || powerBoardError.has_errors) && !inErrorScreen) {
      menu_service::needUpdate = true;
      inErrorScreen = true;
    } else if((!mainBoardError.has_errors && !powerBoardError.has_errors) && inErrorScreen) {
      menu_service::needUpdate = true;
      inErrorScreen = false;
    }
    
    if(menu_service::needUpdate) {
      menu_service::updateScreen();
//      printf("Available memory = %d\r\n\n", Memory::availableMemory(1) );
    }
    
    switch(deviceOpMode.getCurrentMode()) {
      default:
      case NOT_CONFIGURED:
        if(espData.is_configured) {
          printf("NO CONFIG -> HAS CONFIG\r\n");
          deviceOpMode.setConfigured();
          menu_service::needUpdate = true;
          menu_service::resetScreen = true;
        }
        break;
      
      case CONFIGURED:
        if(!espData.is_configured) {
          printf("HAS CONFIG -> NO CONFIG\r\n");
        }
        break;
      
      case WELCOME:
        //nothing to do here
        //wait for user to finish onboarding
        break;
      
      case TEST_MODE:
        if(isFirst) {
          isFirst = false;
          deviceOpMode.setTestMode();
        }
        break;
    }
    
    tempController.updateTemperatures();
    if(!deviceOpMode.isOnboarding()) {
      device_modes::loop();
    }
    __WFI();
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
