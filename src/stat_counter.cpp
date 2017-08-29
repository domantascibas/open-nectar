#include "mbed.h"
#include "data.h"
#include "stat_counter.h"

Timer hour_stat_timer;

namespace stat_counter {
  void hourly_stats() {
    hour_stat_timer.stop();
    float time_passed = hour_stat_timer.read();
    data.hourly_kwh += data.pv_power * time_passed / 3600; 
    printf("hourly energy: %.2f\r\n", data.hourly_kwh);
    hour_stat_timer.reset();
    hour_stat_timer.start();
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
