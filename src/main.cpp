#include "mbed.h"
#include "hardware.h"
#include "power_board.h"
#include "esp.h"
#include "device_modes.h"
#include "service.h"
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
  service::setup();
  hardware::setup();
  wait(2.0);
  power_board::setup();
  esp::setup();
  
  menu_service::updated = true;

  while(1) {
    if(menu_service::updated) {
      menu_service::updateScreen();
      printf("Available memory = %d\r\n\n", availableMemory(1) );
    }
    service::loop();
    hardware::updateTemperature();
    power_board::loop();
    device_modes::loop();

    __WFI();
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
