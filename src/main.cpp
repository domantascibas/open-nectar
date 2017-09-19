#include "mbed.h"
#include "hardware.h"
#include "power_board.h"
#include "esp.h"
#include "device_modes.h"
#include "service.h"
#include "screen.h"
//#include "menu_service.h"

int main() {
//  menu_service::setup();
  service::setup();
  screen::setup();
  hardware::setup();
  wait(2.0);
//  power_board::setup();
//  esp::setup();

  while(1) {
    screen::loop();
//    hardware::loop();
//    power_board::loop();
//    device_modes::loop();
//    service::loop();
//    __WFI();
    wait(1);
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
