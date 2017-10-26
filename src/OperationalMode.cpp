#include "OperationalMode.h"
#include "mbed.h"

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
  if(!isConfigured) {
    printf("NOT CONFIGURED Heater Mode from local\r\n");
    return data.current_mode;
  }
  else {
    printf("CONFIGURED Heater Mode from ESP\r\n");
    return (nectar_contract::HeaterMode)espDeviceData.heater_mode;
  }
}
