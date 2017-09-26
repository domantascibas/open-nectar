#include "mbed.h"
#include "data.h"
#include "stat_counter.h"

//TODO move boiler power to the Data struct
//TODO save grid_kwh and solar_kwh to flash regularly (once a day?)

Timer stat_timer;
static const float boiler_power = 2.0;

namespace stat_counter {
  void increase() {
    stat_timer.stop();
    float time_passed = stat_timer.read();
    if(data.grid_relay_on) {
      data.grid_kwh += boiler_power * time_passed / 3600 / 1000;
    } else if(data.sun_relay_on) {
      if(data.pv_power > 0) {
        data.solar_kwh += data.pv_power * time_passed / 3600 / 1000;
      }
    }
    printf("solar: %.2fkWh grid: %.2fkWh\r\n", data.solar_kwh, data.grid_kwh);
    stat_timer.reset();
    stat_timer.start();
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
