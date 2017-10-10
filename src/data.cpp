#include "mbed.h"
#include "data.h"
#include "NectarStream.h"
#include "NectarContract.h"

//default init values
bool line_busy = false;

HeaterMode current_mode = None;
HeaterMode previous_mode = None;
const float boiler_power = 0.0;

float temp_max = 75.0;
float temp_min = 5.0;
float temp_scheduled = 40.0;
float temp_scheduled_night = 30.0;
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
float radiator_temp = 0.0;
float device_temperature = 0.0;
uint8_t mosfet_overheat_on = false;
uint8_t power_board_error = 0x00;

bool calibrated = false;
float solar_kwh = 0.0;
float grid_kwh = 0.0;
uint8_t relay_state;
bool has_config = false;
bool has_internet = false;

Data data = {
  current_mode, previous_mode, boiler_power, temp_max, temp_min, temp_scheduled, temp_scheduled_night, temp_away, temp_boiler,
  sun_relay_on, grid_relay_on, generator_on,
  pv_power, pv_voltage, pv_current, pv_ref_voltage, pv_ref_current, pwm_duty,
  airgap_temp, radiator_temp, device_temperature, mosfet_overheat_on,
  power_board_error, calibrated, solar_kwh, grid_kwh, relay_state, has_config, has_internet
};

nectar_contract::NectarStatus EspDeviceData = {
  nectar_contract::None, true, has_internet, temp_scheduled, temp_max, 2.2
};

namespace power_data {
  nectar_contract::PowerBoardStats stats = {
    data.pv_power, data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.power_board_error, data.calibrated, data.generator_on,
    data.solar_kwh
  };
  
  nectar_contract::PowerBoardGridMeter grid_meter = {
    data.grid_kwh
  };
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
