#include "DataService.h"
#include "device_modes.h"

TemperatureData temperatureData;

namespace DataService {
  nectar_contract::HeaterMode currentHeaterMode = nectar_contract::None;
  nectar_contract::HeaterMode previousHeaterMode = nectar_contract::None;
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
  float solar_kwh_today = 0.0;
  float solar_kwh_today_diff = 0.0;
  float grid_kwh = 0.0;
  float ref_voltage = 0.0;
  float ref_current = 0.0;
  
  bool gridRelayOn = false;
  bool sunRelayOn = false;
};
    
nectar_contract::PowerBoardState powerData = {
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
  temperatureData.getTemperature(),
  temperatureData.getMaxTemperature(),
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
  return dayTemperature;
}

float TemperatureData::getNightTemperature() {
  return nightTemperature;
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

nectar_contract::HeaterMode DataService::getCurrentHeaterMode() {
  if(espData.is_configured) {
    printf("esp::CurrentHeaterMode() = %d\r\n", (nectar_contract::HeaterMode)currentHeaterMode);
    return (nectar_contract::HeaterMode)espData.heater_mode;
  } else {
    printf("data::CurrentHeaterMode() = %d\r\n", (nectar_contract::HeaterMode)currentHeaterMode);
    return (nectar_contract::HeaterMode)currentHeaterMode;
  }
}

nectar_contract::HeaterMode DataService::getPreviousHeaterMode() {
  return (nectar_contract::HeaterMode)previousHeaterMode;
}

void DataService::setCurrentHeaterMode(nectar_contract::HeaterMode mode) {
  previousHeaterMode = currentHeaterMode;
  currentHeaterMode = mode;
}

void DataService::setPreviousHeaterMode() {
  deviceOpMode.setBoostOff(true);
  esp::get_data_ISR();
  if(espData.is_configured) {
    currentHeaterMode = (nectar_contract::HeaterMode)espData.heater_mode;
  } else {
    currentHeaterMode = previousHeaterMode;
  }
}

bool DataService::isGridRelayOn() {
  return device_modes::isGridRelayOn();
}

bool DataService::isSunRelayOn() {
  return device_modes::isSunRelayOn();
}

void DataService::calculateSolarKwhDiff(bool first) {
  if(first)
    solar_kwh_today_diff = powerData.sun_meter_kwh;
  solar_kwh_today = powerData.sun_meter_kwh - solar_kwh_today_diff;
}

float DataService::getSolarKwhToday() {
  return solar_kwh_today;
}
