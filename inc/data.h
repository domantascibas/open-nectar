#ifndef DATA_H
#define DATA_H

#include "NectarStream.h"

//Constants
#define DRIVER_ON                   0
#define DRIVER_OFF                  1

enum HeaterMode {
  None,
  Boost,
  Away,
  Nogrid,
  Count
};

enum codes {
  NS_OK,
  NS_ERROR,
  MSG_ERROR
};

struct Data {
  HeaterMode current_mode;
  HeaterMode previous_mode;
  
  const float boiler_power;

  float temp_max;
  float temp_min;
  float temp_scheduled;
  float temp_scheduled_night;
  float temp_away;
  float temp_boiler;

  bool sun_relay_on;
  bool grid_relay_on;
  bool generator_on;

  float pv_power;
  float pv_voltage;
  float pv_current;
  float pv_ref_voltage;
  float pv_ref_current;

  float pwm_duty;
  float airgap_temp;
  float device_temperature;
  uint8_t mosfet_overheat_on;
  uint8_t power_board_error;
  
  bool calibrated;
  float solar_kwh;
  float solar_kwh_today;
  float d_kwh;
  float grid_kwh;
  uint8_t relay_state;
  bool has_config;
  bool has_internet;
};

extern Data data;
extern nectar_contract::NectarStatus EspDeviceData;
extern bool line_busy;

namespace power_data {
  extern nectar_contract::PowerBoardStats stats;
  extern nectar_contract::PowerBoardGridMeter grid_meter;
}

#endif

// *******************************Nectar Sun Copyright � Nectar Sun 2017*************************************
