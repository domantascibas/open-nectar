#include "mbed.h"
#include "hardware.h"
#include "EspComms.h"
#include "PowerBoardComms.h"
#include "device_modes.h"
#include "service.h"
#include "menu_service.h"
#include "OperationalMode.h"

int main() {
  static bool isFirst = true;
  service::setup();
  menu_service::setup();
  hardware::setup();
  wait(2.0);
  hardware::updateTemperature();
  power_board::setup();
  esp::setup();
  device_modes::setup();
  
  menu_service::needUpdate = true;

  while(1) {
    if(menu_service::needUpdate) {
      menu_service::updateScreen();
//      printf("Available memory = %d\r\n\n", Memory::availableMemory(1) );
    }
    
    switch(deviceOpMode.currentMode) {
      default:
      case NOT_CONFIGURED:
        if(espDeviceData.is_configured) deviceOpMode.setConfigured();
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

    hardware::updateTemperature();
    __WFI();
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
