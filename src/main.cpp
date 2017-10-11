#include "mbed.h"
#include "device_modes.h"
#include "pwm.h"
#include "main_board.h"
#include "measurements.h"
#include "data.h"
#include "service.h"
#include "error_handler.h"

int main() {
  service::setup();
  main_board::setup();
  pwm::setup();
  device_modes::setup();
  sensors::setup();
  DEBUG_PRINT("Energy Meters: %.4f, %.4f\r\n", data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
  printf("SETUP DONE\r\n");
  
  while(1) {
    if(NectarError.has_error(CALIBRATION_ERROR)) {
      sensors::loop();
      __WFI();
    } else {
        device_modes::loop();
        sensors::loop();
        __WFI();
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
