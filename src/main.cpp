#include "mbed.h"
#include "hardware.h"
#include "esp.h"
#include "power_board.h"
#include "device_modes.h"
#include "service.h"
#include "state_service.h"
#include "menu_service.h"

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
    
    switch(StateService::currentDeviceState) {
      default:
      case NOT_CONFIGURED:
        if(data.has_config) StateService::currentDeviceState = CONFIGURED;
        device_modes::loop();
        power_board::loop();
        break;
      
      case CONFIGURED:
        device_modes::loop();
        power_board::loop();
        break;
      
      case WELCOME:
        break;
      
      case TEST_MODE:
        if(isFirst && !line_busy) {
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
