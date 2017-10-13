#include "mbed.h"
#include "hardware.h"
#include "esp.h"
#include "power_board.h"
#include "device_modes.h"
#include "service.h"
#include "state_service.h"
#include "menu_service.h"

#include "memory_checker.h"
//#include "button.h"

//void test_ISR() {
//  printf("test isr\r\n");
//}

//Button key1(PA_5, &test_ISR);

int main() {
  StateService::setup();
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
    StateService::loop();
    if(menu_service::needUpdate) {
      menu_service::updateScreen();
    }
    hardware::updateTemperature();
    device_modes::loop();
    power_board::loop();

    __WFI();
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
