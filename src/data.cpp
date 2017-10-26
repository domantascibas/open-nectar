#include "mbed.h"
#include "data.h"

//default init values

nectar_contract::HeaterMode current_mode = nectar_contract::None;
nectar_contract::HeaterMode previous_mode = nectar_contract::None;
float boiler_power = 0.0;

float temp_max = 75.0;
float temp_min = 5.0;
float temp_scheduled = 55.0;
float temp_scheduled_night = 40.0;
float temp_away = 10.0;
float temp_boiler = 0.0;

bool sun_relay_on = false;
bool grid_relay_on = false;
bool generator_on = false;

float pv_power = 0.0;
float pv_voltage = 0.0;
float pv_current = 0.0;
float pv_ref_voltage = 0.0;
float pv_ref_current = 0.0;
float pwm_duty = 0.1;

float airgap_temp = 0.0;
float device_temperature = 0.0;
uint8_t mosfet_overheat_on = false;
uint32_t power_board_error = 0;

bool calibrated = false;
float solar_kwh = 0.0;
float solar_kwh_today = 0.0;
float d_kwh = 0.0;
float grid_kwh = 0.0;
uint8_t relay_state;
bool has_config = false;
bool has_internet = false;
bool start_power_board = false;

bool reset = false;
bool boost_off = false;

Data data = {
  current_mode, previous_mode, boiler_power, temp_max, temp_min, temp_scheduled, temp_scheduled_night, temp_away, temp_boiler,
  sun_relay_on, grid_relay_on, generator_on,
  pv_power, pv_voltage, pv_current, pv_ref_voltage, pv_ref_current, pwm_duty,
  airgap_temp, device_temperature, mosfet_overheat_on,
  power_board_error, calibrated, solar_kwh, solar_kwh_today, d_kwh, grid_kwh, relay_state, has_config, has_internet,
  start_power_board, reset, boost_off
};

nectar_contract::ESPState espDeviceData = {
  current_mode,
  has_config,
  has_internet,
  temp_scheduled,
  temp_max,
  boiler_power,
  0
};

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
