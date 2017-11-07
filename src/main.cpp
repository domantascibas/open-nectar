#include "mbed.h"
#include "EspComms.h"
#include "PowerBoardComms.h"
#include "device_modes.h"
#include "menu_service.h"
#include "ErrorHandler.h"
#include "TemperatureController.h"
#include "DataService.h"

static const PinName TX = PA_2;
static const PinName RX = PA_3;
static const uint32_t SERIAL_BAUDRATE = 115200;
bool inErrorScreen = false;

RawSerial pc(TX, RX);
TemperatureController tempController;

int main() {
  static bool isFirst = true;
  pc.baud(SERIAL_BAUDRATE);
  pc.printf("[COMMS] Started\r\n");
  mainBoardError.save_error_code(0x300);
  
  menu_service::setup();
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
    } else {
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
