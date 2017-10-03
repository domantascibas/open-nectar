#include "mbed.h"
#include "data.h"
#include "NectarStream.h"
#include "NectarContract.h"

bool line_busy = false;

MODE current_mode = MODE_DEFAULT;
float temp_max = 70.0;
float temp_min = 5.0;
float temp_scheduled = 40.0;
float temp_scheduled_night = 30.0;
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
float solar_kwh = 0.0;
float grid_kwh = 0.0;

Data data = { 
  current_mode, temp_max, temp_min, temp_scheduled, temp_scheduled_night, temp_away, temp_boiler,
  sun_relay_on, grid_relay_on, generator_on,
  pv_power, pv_voltage, pv_current, pv_ref_voltage, pv_ref_current, pwm_duty,
  airgap_temp, radiator_temp, device_temperature, mosfet_overheat_on,
  0x00, calibrated, solar_kwh, grid_kwh
};

namespace power_data {
  nectar_contract::PowerBoardStats stats = {
    data.pv_power, data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.error, data.calibrated, data.generator_on,
    data.solar_kwh
  };
  
  nectar_contract::PowerBoardGridMeter grid_meter = {
    data.grid_kwh
  };
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
