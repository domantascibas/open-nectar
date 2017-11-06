#include "DeviceMode.h"
#include "device_modes.h"

DeviceMode deviceOpMode;

uint8_t DeviceMode::getCurrentMode() {
  return currentMode;
}

void DeviceMode::setConfigured() {
  currentMode = CONFIGURED;
  configured = true;
}

void DeviceMode::resetConfiguration() {
  currentMode = WELCOME;
  testMode = false;
  configured = false;
  onboarding = true;
  device_modes::reset();
}

void DeviceMode::setTestMode() {
  testMode = true;
  endOnboarding();
  currentMode = TEST_MODE;
}

void DeviceMode::endOnboarding() {
  if(!configured) currentMode = NOT_CONFIGURED;
  else currentMode = CONFIGURED;
  onboarding = false;
}

bool DeviceMode::isInTestMode() {
  return testMode;
}

bool DeviceMode::isOnboarding() {
  return onboarding;
}

bool DeviceMode::isConfigured() {
  return configured;
}

bool DeviceMode::isPairing() {
  return pairing;
}

bool DeviceMode::isReset() {
  return reset;
}

bool DeviceMode::isBoostOff() {
  return boostOff;
}

void DeviceMode::setPairing(bool input) {
  pairing = input;
}

void DeviceMode::setReset(bool input) {
  reset = input;
}

void DeviceMode::setBoostOff(bool input) {
  boostOff = input;
}

//time_t OperationalMode::timeHMtoTime(time_hm timeHM) {
//  time_t rtc = time(NULL);
//  
//  return rtc - (rtc % (24 * 60 * 60)) + timeHM.hours * 60 * 60 + timeHM.minutes * 60;
//}

//float OperationalMode::getTemperature() {
//  if(!isConfigured) {
//    time_hm currentTime = menu_actions::getTime(Current);
//    time_hm dayStartTime = menu_actions::getTime(DayStart);
//    time_hm dayEndTime = menu_actions::getTime(NightStart);
//    
//    if((timeHMtoTime(currentTime) > timeHMtoTime(dayStartTime)) && (timeHMtoTime(currentTime) < timeHMtoTime(dayEndTime)))
//      return data.temp_scheduled;
//    else return data.temp_scheduled_night;
//  }
//  else return espData.temperature;
//}

//float OperationalMode::getTemperatureMax() {
//  if(!isConfigured) return data.temp_max;
//  else return espData.temperature_max;
//}

//nectar_contract::HeaterMode OperationalMode::getHeaterMode() {
//  if(!isConfigured) {
//    printf("NOT CONFIGURED Heater Mode from local\r\n");
//    return data.current_mode;
//  }
//  else {
//    printf("CONFIGURED Heater Mode from ESP\r\n");
//    return (nectar_contract::HeaterMode)espData.heater_mode;
//  }
//}