#include "mbed.h"
#include "hardware.h"
#include "power_board.h"
#include "esp.h"
#include "device_modes.h"
#include "service.h"

int main() {
  service::setup();
  hardware::setup();
  wait(2.0);
  power_board::setup();
  esp::setup();

  while(1) {
    hardware::loop();
    power_board::loop();
    device_modes::loop();
//    service::loop();
    __WFI();
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
