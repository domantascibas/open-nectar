#include "mbed.h"
#include "hardware.h"
#include "esp.h"
#include "power_board.h"
#include "device_modes.h"
#include "service.h"
#include "state_service.h"
#include "menu_service.h"

// Function prototype
 
int availableMemory(int = 256, int = 0x8000, bool = true);
 
// Calculation function 
 
int availableMemory(int resolution, int maximum, bool disableInterrupts) {
  if(resolution < 1) resolution = 1;
  if(maximum < 0) maximum = 0;

  int low = 0;
  int high = maximum + 1;

  if(disableInterrupts) __disable_irq();
  while(high - low > resolution) {
    int mid = (low + high) / 2;
    void* p = malloc(mid);
    if(p == NULL) {
      high = mid;
    } else {
      free(p);
      low = mid;
    }
  }
  if(disableInterrupts) __enable_irq();
  return low;
}
 
// Calling the function
 
int main() {
  menu_service::setup();
  StateService::setup();
  service::setup();
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
      printf("Available memory = %d\r\n\n", availableMemory(1) );
//          printf("daytemp: %.2f nightemp: %.2f\r\n", data.temp_scheduled, data.temp_scheduled_night);
    }
    hardware::updateTemperature();
    device_modes::loop();
    power_board::loop();

    __WFI();
  }
}

// *******************************Nectar Sun Copyright � Nectar Sun 2017*************************************   
