#include "TemperatureController.h"
#include "ErrorHandler.h"
#include "data.h"

static const PinName BOILER_TEMP_PROBE = PB_8;
static const PinName DEVICE_TEMP_PROBE = PB_9;

static const float WATER_TEMPERATURE_LIMIT_MIN = 10.0;
static const float WATER_TEMPERATURE_LIMIT_MAX = 90.0;
static const float DEVICE_TEMPERATURE_LIMIT_MAX = 95.0;

TemperatureSensor boilerTemp(BOILER_TEMP_PROBE, 20);
TemperatureSensor deviceTemp(DEVICE_TEMP_PROBE, 10);

void TemperatureController::init() {
  boilerTemp.init();
  deviceTemp.init();
  
  if(boilerTemp.isSensorFound()) {
    mainBoardError.clear_error(NO_BOILER_TEMP);
  }
  
  if(deviceTemp.isSensorFound()) {
    mainBoardError.clear_error(DEVICE_OVERHEAT);
  }
}

float TemperatureController::getBoilerTemperature() {
  if(boilerTemp.isNewValueAvailable()) {
    boilerTemperature = boilerTemp.getTemperature();
    if(boilerTemperature > WATER_TEMPERATURE_LIMIT_MAX) {
      mainBoardError.set_error(MAX_TEMPERATURE);
    } else if(boilerTemperature < WATER_TEMPERATURE_LIMIT_MIN) {
      mainBoardError.set_error(MIN_TEMPERATURE);
    } else {
      if(mainBoardError.has_error(MAX_TEMPERATURE)) mainBoardError.clear_error(MAX_TEMPERATURE);
      if(mainBoardError.has_error(MIN_TEMPERATURE)) mainBoardError.clear_error(MIN_TEMPERATURE);
      printf("[TEMPERATURE] boiler %.2f\r\n", boilerTemperature);
    }
  }
  return boilerTemperature;
}

float TemperatureController::getDeviceTemperature() {
  if(deviceTemp.isNewValueAvailable()) {
    deviceTemperature = deviceTemp.getTemperature();
    if(deviceTemperature > DEVICE_TEMPERATURE_LIMIT_MAX) {
      mainBoardError.set_error(DEVICE_OVERHEAT);
    } else {
      if(mainBoardError.has_error(DEVICE_OVERHEAT)) mainBoardError.clear_error(DEVICE_OVERHEAT);
      printf("[TEMPERATURE] internal %.2f\r\n", deviceTemperature);
    }
  }
  return deviceTemperature;
}

void TemperatureController::updateTemperatures(float *ptrBoilerTemp, float *ptrDeviceTemp) {
  *ptrBoilerTemp = getBoilerTemperature();
  *ptrDeviceTemp = getDeviceTemperature();
}
