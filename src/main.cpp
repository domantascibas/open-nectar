#include "mbed.h"
#include "device_modes.h"
#include "main_board.h"
#include "data.h"
#include "service.h"
#include "ErrorHandler.h"

#define VERSION_MAJOR         1
#define VERSION_MINOR         0
#define VERSION_REVISION      1

int main() {
  service::setup();
  printf("Starting device modes\r\n");
  device_modes::setup();
  DEBUG_PRINT("Energy Meters: %.4f, %.4f\r\n", data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
  main_board::setup();
  printf("SETUP DONE\r\n");
  
  while(1) {
    if(!nectarError.has_error(CALIBRATION_ERROR)) device_modes::loop();
    device_modes::calibrate_device();
    __WFI();
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
