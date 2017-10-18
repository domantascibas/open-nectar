#include "mbed.h"
#include "data.h"
#include "stat_counter.h"
#include "OperationalMode.h"

Timer stat_timer;

namespace stat_counter {
  void setup() {
    stat_timer.start();
  }
  
  void increase() {
    if(data.grid_relay_on && !deviceOpMode.inTestMode) {
      float time_passed = stat_timer.read();
      stat_timer.reset();
      data.grid_kwh += data.boiler_power * time_passed / 3600 / 1000;
    } else {
      stat_timer.reset();
    }
    printf("solar: %.2fkWh grid: %.2fkWh\r\n", data.solar_kwh, data.grid_kwh);
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
