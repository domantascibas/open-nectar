#ifndef DATA_H
#define DATA_H

#include "NectarContract.h"

enum HeaterMode {
  None,
  Boost,
  Away,
  Nogrid,
  Count
};

struct Data {
  nectar_contract::HeaterMode current_mode;
  nectar_contract::HeaterMode previous_mode;

  float temp_max;
  float temp_min;
  float temp_scheduled;
  float temp_scheduled_night;
  float temp_away;
  float temp_boiler;

  bool sun_relay_on;
  bool grid_relay_on;
  float airgap_temp;
  float device_temperature;

  float solar_kwh_today;
  float d_kwh;
  float grid_kwh;

  bool reset;
  bool boost_off;
};

extern Data data;

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************
