#include "OperationalMode.h"
#include "EspComms.h"

OperationalMode deviceOpMode;

uint8_t OperationalMode::getCurrentMode() {
  return currentMode;
}

void OperationalMode::setConfigured() {
  currentMode = CONFIGURED;
  isConfigured = true;
}

void OperationalMode::setTestMode() {
  inTestMode = true;
  endOnboarding();
  currentMode = TEST_MODE;
}

void OperationalMode::endOnboarding() {
  if(!isConfigured) currentMode = NOT_CONFIGURED;
  else currentMode = CONFIGURED;
  inOnboarding = false;
}

time_t OperationalMode::timeHMtoTime(time_hm timeHM) {
  time_t rtc = time(NULL);
  
  return rtc - (rtc % (24 * 60 * 60)) + timeHM.hours * 60 * 60 + timeHM.minutes * 60;
}

float OperationalMode::getTemperature() {
  if(!isConfigured) {
    time_hm currentTime = menu_actions::getTime(Current);
    time_hm dayStartTime = menu_actions::getTime(DayStart);
    time_hm dayEndTime = menu_actions::getTime(NightStart);
    
    if((timeHMtoTime(currentTime) > timeHMtoTime(dayStartTime)) && (timeHMtoTime(currentTime) < timeHMtoTime(dayEndTime)))
      return data.temp_scheduled;
    else return data.temp_scheduled_night;
  }
  else return esp::espData.temperature;
}

float OperationalMode::getTemperatureMax() {
  if(!isConfigured) return data.temp_max;
  else return esp::espData.temperature_max;
}

nectar_contract::HeaterMode OperationalMode::getHeaterMode() {
  if(!isConfigured) {
    printf("NOT CONFIGURED Heater Mode from local\r\n");
    return data.current_mode;
  }
  else {
    printf("CONFIGURED Heater Mode from ESP\r\n");
    return (nectar_contract::HeaterMode)esp::espData.heater_mode;
  }
}
