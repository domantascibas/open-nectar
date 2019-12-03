#ifndef NECTAR_CONTRACT_H
#define NECTAR_CONTRACT_H

#include <stdint.h>
#include <time.h>

// Baud rate for serial communication
static const uint16_t C_SERIAL_BAUD_RATE = 19200;

namespace nectar_contract
{
struct MainBoardStateForESP
{
  float moment_sun_watt;
  uint8_t grid_relay_on;
  float water_temperature;
  uint8_t sun_relay_on;
  float sun_voltage;
  float sun_current;
  float device_temperature;
  uint8_t transistor_overheat_on;
  float pwm_duty;
  uint32_t power_board_error_code;
  uint32_t main_board_error_code;
  float sun_meter_kwh;
  float grid_meter_kwh;
  time_t time;
  uint8_t reset;
  uint8_t pair_mode;
  uint8_t send_boost_off;
	float ref_voltage;
	float ref_current;
	uint16_t main_version;
};

enum HeaterMode
{
  None = 0,
  Boost = 1,
  Away = 2,
  Nogrid = 3,
	Alloff = 4
};

struct ESPState
{
  uint8_t heater_mode;
  uint8_t is_configured;
  uint8_t has_internet_connection;
  float temperature;
  float temperature_max;
  float boiler_power;
  uint64_t sync_time;
  uint16_t pin;
	float esp_version;
};

struct MainBoardStateForPower
{
  float grid_meter_kwh;
  uint8_t start;
  uint8_t is_test_mode_on;
  uint8_t is_in_onboarding;
	uint8_t calibrate;
};

struct PowerBoardState
{
  float sun_power;
  float sun_voltage;
  float sun_current;
  float pwm_duty;
  float device_temperature;
  uint8_t transistor_overheat_on;
  uint32_t power_board_error_code;
  uint8_t device_calibrated;
  uint8_t pwm_generator_on;
  float sun_meter_kwh;
  float grid_meter_kwh;
  float ref_voltage;
  float ref_current;
	float power_version;
};
} // namespace nectar_contract

#endif
