#include "mbed.h"
#include "hardware.h"
#include "device_modes.h"
#include "pwm.h"
#include "main_board.h"
#include "measurements.h"
#include "data.h"

//PC_9 voltage monitor. RESET active - low. normal operation - 1, when <8.5V - 0.

uint8_t self_check() {
  
}

int main() {
  main_board::setup();
  hardware::setup();
  pwm::setup();
  device_modes::setup();
  sensors::setup();
  printf("SETUP DONE\r\n");
  printf("starting program loops\r\n");
  
  while(1) {
    main_board::loop();
    device_modes::loop();
    sensors::loop();
    hardware::loop();
    __WFI();
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
