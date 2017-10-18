#include "mbed.h"
#include "data.h"

bool isTestMode = false;
uint8_t current_state = IDLE;
  
float moment_power = 0.00;
float moment_voltage = 0.00;
float moment_current = 0.00;

float reference_voltage = 0.00;
float reference_current = 0.00;
  
float grid_energy_meter_kwh = 0.00;
float sun_energy_meter_kwh = 0.00;

float pwm_duty = 0.1;
float airgap_temperature = 0.00;

bool mosfet_overheat_on = false;
bool calibrated = false;
bool generator_on = false;
  
uint8_t power_board_error = 0x00;
  
Data data = {
  isTestMode, current_state, moment_power, moment_voltage, moment_current, reference_voltage, reference_current, grid_energy_meter_kwh, sun_energy_meter_kwh,
  pwm_duty, airgap_temperature, mosfet_overheat_on, calibrated, generator_on, power_board_error
};

StorageData storage_data = {
  reference_voltage, reference_current, sun_energy_meter_kwh, grid_energy_meter_kwh
};

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
