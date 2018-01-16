#include "DataService.h"
#include "device_modes.h"
#include "../submodules/device_menu/src/helpers/helpers.h"
#include "menu_service.h"
#include "Storage.h"

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
  temperatureData.getDeviceTemperature(),
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

time_t timeHMtoTime(time_hm timeHM) {
  time_t rtc = time(NULL);
  
  return rtc - (rtc % (24 * 60 * 60)) + timeHM.hours * 60 * 60 + timeHM.minutes * 60;
}

void TemperatureData::setDayTemperature(float temp) {
  dayTemperature = temp;
}

void TemperatureData::setNightTemperature(float temp) {
  nightTemperature = temp;
}

void TemperatureData::setMaxTemperature(float temp) {
  maxTemperature = temp;
  if(dayTemperature > maxTemperature) dayTemperature = maxTemperature;
  if(nightTemperature > maxTemperature) nightTemperature = maxTemperature;
}

void TemperatureData::setBoilerTemperature(float temp) {
  boilerTemperature = temp;
}

void TemperatureData::setDeviceTemperature(float temp) {
  deviceTemperature = temp;
}

float TemperatureData::getTemperature() {
  if(deviceOpMode.isConfigured()) return espData.temperature;
  else {
    time_hm currentTime = menu_actions::getTime(Current);
    time_hm dayStartTime = menu_actions::getTime(DayStart);
    time_hm dayEndTime = menu_actions::getTime(NightStart);
    
    if((timeHMtoTime(currentTime) > timeHMtoTime(dayStartTime)) && (timeHMtoTime(currentTime) < timeHMtoTime(dayEndTime)))
      return dayTemperature;
    else return nightTemperature;
  }
}

float TemperatureData::getDayTemperature() {
  return dayTemperature;
}

float TemperatureData::getNightTemperature() {
  return nightTemperature;
}

float TemperatureData::getMinTemperature() {
  return minTemperature;
}

float TemperatureData::getMaxTemperature() {
  if(deviceOpMode.isConfigured()) return espData.temperature_max;
  else return maxTemperature;
}

float TemperatureData::getBoilerTemperature() {
  return boilerTemperature;
}


float TemperatureData::getDeviceTemperature() {
  return powerData.device_temperature;
}

nectar_contract::HeaterMode DataService::getCurrentHeaterMode() {
  if(espData.has_internet_connection && !deviceOpMode.isInTestMode()) {
    return (nectar_contract::HeaterMode)espData.heater_mode;
  } else {
    return (nectar_contract::HeaterMode)currentHeaterMode;
  }
}

nectar_contract::HeaterMode DataService::getPreviousHeaterMode() {
  return (nectar_contract::HeaterMode)previousHeaterMode;
}

void DataService::resetData() {
  currentHeaterMode = nectar_contract::None;
  previousHeaterMode = nectar_contract::None;
  
  hasConfig = false;
  hasInternet = false;
  
  temperatureData.setDayTemperature(55.0);
  temperatureData.setNightTemperature(40.0);
  temperatureData.setMaxTemperature(75.0);
  
  boiler_power = 0;
  
  espData.heater_mode = currentHeaterMode;
  espData.is_configured = hasConfig;
  espData.has_internet_connection = hasInternet;
  espData.temperature = temperatureData.getTemperature();
  espData.temperature_max = temperatureData.getMaxTemperature();
  espData.boiler_power = boiler_power;
  espData.sync_time = 0;
}

void DataService::updateHeaterMode(nectar_contract::HeaterMode currMode, nectar_contract::HeaterMode prevMode) {
  previousHeaterMode = prevMode;
  currentHeaterMode = currMode;
}

void DataService::setCurrentHeaterMode(nectar_contract::HeaterMode mode) {
  previousHeaterMode = currentHeaterMode;
  currentHeaterMode = mode;
  device_modes::updateHeaterMode = true;
  Storage::saveHeaterMode(currentHeaterMode, previousHeaterMode);
}

void DataService::setPreviousHeaterMode() {
  deviceOpMode.setBoostOff(true);
  esp::get_data_ISR();
  if(espData.is_configured) {
    currentHeaterMode = (nectar_contract::HeaterMode)espData.heater_mode;
  } else {
    currentHeaterMode = previousHeaterMode;
  }
  Storage::saveHeaterMode(currentHeaterMode, previousHeaterMode);
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
  if(solar_kwh_today < 0) solar_kwh_today = 0;
}

float DataService::getSolarKwhToday() {
  return solar_kwh_today;
}

float DataService::getBoilerPower() {
  return espData.boiler_power;
}
