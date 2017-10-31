#include "data.h"

//default init values

nectar_contract::HeaterMode current_mode = nectar_contract::None;
nectar_contract::HeaterMode previous_mode = nectar_contract::None;

float temp_max = 75.0;
float temp_min = 5.0;
float temp_scheduled = 55.0;
float temp_scheduled_night = 40.0;
float temp_boiler = 0.0;

bool sun_relay_on = false;
bool grid_relay_on = false;

float device_temperature = 0.0;
float solar_kwh_today = 0.0;
float d_kwh = 0.0;
float grid_kwh = 0.0;

bool reset = false;
bool boost_off = false;

Data data = {
  current_mode, previous_mode, temp_max, temp_min, temp_scheduled, temp_scheduled_night, temp_boiler,
  sun_relay_on, grid_relay_on,
  device_temperature,
  solar_kwh_today, d_kwh, grid_kwh,
  reset, boost_off
};

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
