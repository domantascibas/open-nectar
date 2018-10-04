#include "consts.h"
#include "ServiceComms.h"
#include "EspComms.h"
#include "PowerBoardComms.h"
#include "device_modes.h"
#include "menu_service.h"
#include "error_controller.h"
#include "TemperatureController.h"
#include "DataService.h"
#include "Storage.h"
#include "CommsController.h"
#include "watchdog_timer.h"

bool inErrorScreen = false;

TemperatureController tempController;
Timer comms_timeout;

int main() {
	float current_s;
  static bool isFirst = true;
	error_controller_init();
  Storage::init();
  
  service::setup();
  
  menu_service::setup();
  menu_service::updateScreen();
  wait(1.0);
  tempController.init();
  wait(0.5);
  tempController.updateTemperatures();
  wait(0.5);

	menu_service::updateScreen();
	wait(0.5);
	esp::setup();
	wait(0.5);
	power_board::setup();
	
	comms_timeout.start();
  current_s = comms_timeout.read();
	while((!power_board::receivedFirstMessage() || (powerData.power_version == 0.00)) && (current_s < POWER_INIT_TIMEOUT)) {
		__WFI();
		current_s = comms_timeout.read();
	}
	comms_timeout.stop();
	comms_timeout.reset();
	menu_service::updateScreen();
  wait(1.5);
	
	menu_service::updateScreen();
	comms_timeout.start();
	current_s = comms_timeout.read();
  while((!esp::receivedFirstMessage() || (espData.esp_version == 0.00)) && (current_s < ESP_INIT_TIMEOUT)) {
		__WFI();
		current_s = comms_timeout.read();
	}
	comms_timeout.stop();
	comms_timeout.reset();
	menu_service::updateScreen();
  wait(1.5);
  
	menu_service::updateScreen();
	device_modes::setup();
  wait(2);
	
	watchdog_timer_init();
  deviceOpMode.endLoading();
  
  while(1) {
    watchdog_timer_update();
    
    if(isFirst) {
      isFirst = false;
      if(Storage::isConfigured()) {
        printf("[CONFIG] loading data from storage\r\n");
        Storage::loadConfigData();
        if(Storage::isEspConfigured()) {
          espData.is_configured = true;
          deviceOpMode.setConfigured();
        }
        deviceOpMode.endOnboarding();
      }
      menu_service::needUpdate = true;
    }
    
    if(menu_service::needUpdate) {
      menu_service::updateScreen();
    }
    
    switch(deviceOpMode.getCurrentMode()) {
      default:
      case NOT_CONFIGURED:        // ESP NOT CONFIGURED
        if(espData.is_configured) {
          printf("NO CONFIG -> HAS CONFIG\r\n");
          deviceOpMode.setConfigured();
          menu_service::needUpdate = true;
          menu_service::resetScreen = true;
          Storage::saveEspConfig();
        }
        break;
      
      case CONFIGURED:            // ESP CONFIGURED
        if(!espData.is_configured) {
          printf("HAS CONFIG -> NO CONFIG\r\n");
        }
        break;
      
      case WELCOME:
        //nothing to do here
        //wait for user to finish onboarding
        if(espData.is_configured || Storage::isEspConfigured()) {
          deviceOpMode.endOnboarding();
          printf("NO CONFIG -> HAS CONFIG\r\n");
          menu_service::needUpdate = true;
          menu_service::resetScreen = true;
        }
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
    
    commsController.clearQueue();
    
    __WFI();
  }
}
