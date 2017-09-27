#include "mbed.h"
#include "data.h"
#include "NectarStream.h"
#include "NectarContract.h"

bool line_busy = false;

uint8_t current_mode = 0;
float temp_max = 70.0;
float temp_min = 5.0;
float temp_scheduled = 25;
float temp_away = 27.0;
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
float radiator_temp = 0.0;
float device_temperature = 0.0;
uint8_t mosfet_overheat_on = false;
bool calibrated = false;
double solar_kwh = 0.0;
double grid_kwh = 0.0;

Data data = { 
  current_mode, temp_max, temp_min, temp_scheduled, temp_away, temp_boiler,
  sun_relay_on, grid_relay_on, generator_on,
  pv_power, pv_voltage, pv_current, pv_ref_voltage, pv_ref_current, pwm_duty,
  airgap_temp, radiator_temp, device_temperature, mosfet_overheat_on,
  0x00, calibrated, solar_kwh, grid_kwh
};
//Data data = { 0, 65.0, 5.0, 40.0, 0.0, false, false, 0.0, 0.0, 0.0, 0.0, 0.0, false, 0.1, 0.0, 0.0, 0.0, false, 0x00 };

/*
struct PowerBoardStats {
  float sun_voltage;
  float sun_current;
  float pwm_duty;
  float radiator_temperature;
  bool transistor_overheat_on;
  float airgap_temperature;
  uint8_t power_board_error_code;
  uint8_t device_calibrated;
  uint8_t pwm_generator_on;
};
*/

namespace power_data {
  nectar_contract::PowerBoardStats stats = {
    data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp, data.error, data.calibrated, data.generator_on
  };
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
