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
static bool isFirst = true;
bool inErrorScreen = false;

RawSerial pc(TX, RX);
TemperatureController tempController;

int main() {
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
        if(espData.is_configured) deviceOpMode.setConfigured();
        device_modes::loop();
        break;
      
      case CONFIGURED:
        device_modes::loop();
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
        device_modes::loop();
        break;
    }
    
    tempController.updateTemperatures();
    __WFI();
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
