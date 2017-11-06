#include "DataService.h"

TemperatureData temperature;

namespace DataService {
  uint8_t previousHeaterMode = 0;
  uint8_t currentHeaterMode = 0;
  bool hasConfig = false;
  bool hasInternet = false;
  float boiler_power = 0.0;

  float sun_power = 0.0;
  float sun_voltage = 0.0;
  float sun_current = 0.0;
  float pwm_duty = 0.1;
  
  uint8_t mosfet_overheat_on = false;
  uint32_t power_board_error = 0;

  bool calibrated = false;
  bool generator_on = false;
  float solar_kwh = 0.0;
  float grid_kwh = 0.0;
  float ref_voltage = 0.0;
  float ref_current = 0.0;
};
    
nectar_contract::PowerBoardState powerBoardData = {
  DataService::sun_power,
  DataService::sun_voltage,
  DataService::sun_current,
  DataService::pwm_duty,
  DataService::mosfet_overheat_on,
  DataService::power_board_error,
  DataService::calibrated,
  DataService::generator_on,
  DataService::solar_kwh,
  DataService::grid_kwh,
  DataService::ref_voltage,
  DataService::ref_current
};

nectar_contract::ESPState espData = {
  DataService::currentHeaterMode,
  DataService::hasConfig,
  DataService::hasInternet,
  temperature.getTemperature(),
  temperature.getMaxTemperature(),
  DataService::boiler_power,
  0
};

void TemperatureData::setDayTemperature(float temp) {
  dayTemperature = temp;
}

void TemperatureData::setNightTemperature(float temp) {
  nightTemperature = temp;
}

void TemperatureData::setMaxTemperature(float temp) {
  maxTemperature = temp;
}

void TemperatureData::setBoilerTemperature(float temp) {
  boilerTemperature = temp;
}

void TemperatureData::setDeviceTemperature(float temp) {
  deviceTemperature = temp;
}

float TemperatureData::getTemperature() {
  // return day or night temperature
  return dayTemperature;
}

float TemperatureData::getMinTemperature() {
  return minTemperature;
}

float TemperatureData::getMaxTemperature() {
  return maxTemperature;
}

float TemperatureData::getBoilerTemperature() {
  return boilerTemperature;
}


float TemperatureData::getDeviceTemperature() {
  return deviceTemperature;
}

uint8_t DataService::getCurrentHeaterMode() {
  if(espData.is_configured) {
    return espData.heater_mode;
  } else {
    return currentHeaterMode;
  }
}

uint8_t DataService::getPreviousHeaterMode() {
  return previousHeaterMode;
}

void DataService::setCurrentHeaterMode(uint8_t mode) {
  previousHeaterMode = currentHeaterMode;
  currentHeaterMode = mode;
}

void DataService::setPreviousHeaterMode() {
  data.boost_off = true;
  esp::get_data_ISR();
  if(espData.is_configured) {
    currentHeaterMode = espData.heater_mode;
  } else {
    currentHeaterMode = previousHeaterMode;
  }
}
