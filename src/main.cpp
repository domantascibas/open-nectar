#include "mbed.h"
#include "hardware.h"
#include "power_board.h"
#include "esp.h"
#include "device_modes.h"
#include "service.h"
#include "menu_service.h"

int main() {
  menu_service::setup();
  service::setup();
  hardware::setup();
  wait(2.0);
//  power_board::setup();
  esp::setup();

  while(1) {
    if(menu_service::updated) {
      menu_service::updateScreen();
    }
    service::loop();
    hardware::updateTemperature();
//    power_board::loop();
//    device_modes::loop();

    __WFI();
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
