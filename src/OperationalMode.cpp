#include "OperationalMode.h"

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

float OperationalMode::getTemperature() {
  if(!isConfigured) return data.temp_scheduled;
  else return espDeviceData.temperature;
}

float OperationalMode::getTemperatureMax() {
  if(!isConfigured) return data.temp_max;
  else return espDeviceData.temperature_max;
}

nectar_contract::HeaterMode OperationalMode::getHeaterMode() {
  if(!isConfigured) return data.current_mode;
  else return (nectar_contract::HeaterMode)espDeviceData.heater_mode;
}
