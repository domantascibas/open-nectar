#include "mbed.h"
#include "stat_counter.h"
#include "DataService.h"

Timer stat_timer;

namespace stat_counter {
  void setup() {
    stat_timer.start();
  }
  
  void increase() {
    if(DataService::isGridRelayOn() && !deviceOpMode.isInTestMode()) {
      float time_passed = stat_timer.read();
      stat_timer.reset();
      if(time_passed > 0) powerData.grid_meter_kwh += espData.boiler_power * time_passed / 3600 / 1000;
    } else {
      stat_timer.reset();
    }
    printf("solar: %fkWh grid: %fkWh\r\n", powerData.sun_meter_kwh, powerData.grid_meter_kwh);
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
