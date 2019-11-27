#include "mbed.h"
#include "data.h"

bool isCalibrating = false;
bool isInOnboarding = true;
bool isTestMode = false;
uint8_t current_state = IDLE;

// change to uint16_t
// should perform all calculations in uint32_t cast to uint16_t
  
float moment_power = 0.00;
float moment_voltage = 0.00;
float moment_current = 0.00;

float reference_voltage = 0.00;
float reference_current = 0.00;
  
float grid_energy_meter_kwh = 0.00;
float sun_energy_meter_kwh = 0.00;

float pwm_duty = 0.1;
float device_temperature = 0.00;

bool mosfet_overheat_on = false;
bool calibrated = false;
bool generator_on = false;
  
uint32_t power_board_error = 0x00000000;
bool readingSerial = false;
bool readingTemperature = false;
bool safeToReadTemp = false;
bool startCalibration = false;

Data data = {
  isCalibrating, isInOnboarding, isTestMode, current_state, moment_power, moment_voltage, moment_current, reference_voltage, reference_current, grid_energy_meter_kwh, sun_energy_meter_kwh,
  pwm_duty, device_temperature, mosfet_overheat_on, calibrated, generator_on, power_board_error, readingSerial, readingTemperature, safeToReadTemp
};

StorageData storage_data = {
  reference_voltage, reference_current, sun_energy_meter_kwh, grid_energy_meter_kwh
};

nectar_contract::PowerBoardState powerState = {
  0,
  0,
  0,
  0.1,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};
