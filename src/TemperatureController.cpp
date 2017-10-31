#include "TemperatureController.h"
#include "ErrorHandler.h"
#include "data.h"

static const PinName BOILER_TEMP_PROBE = PB_8;
static const PinName DEVICE_TEMP_PROBE = PB_9;

static const float WATER_TEMPERATURE_LIMIT_MIN = 5.0;
static const float WATER_TEMPERATURE_LIMIT_MAX = 90.0;
static const float DEVICE_TEMPERATURE_LIMIT_MAX = 95.0;

TemperatureSensor boilerTemp(BOILER_TEMP_PROBE);
TemperatureSensor deviceTemp(DEVICE_TEMP_PROBE);

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

void TemperatureController::updateTemperatures() {
  if(boilerTemp.isNewValueAvailable()) {
    float newValue = boilerTemp.getTemperature();
    if(newValue > WATER_TEMPERATURE_LIMIT_MAX) {
      mainBoardError.set_error(MAX_TEMPERATURE);
    } else if(newValue < WATER_TEMPERATURE_LIMIT_MIN) {
      mainBoardError.set_error(MIN_TEMPERATURE);
    } else {
      if(mainBoardError.has_error(MAX_TEMPERATURE)) mainBoardError.clear_error(MAX_TEMPERATURE);
      if(mainBoardError.has_error(MIN_TEMPERATURE)) mainBoardError.clear_error(MIN_TEMPERATURE);
      data.temp_boiler = newValue;
      printf("[TEMPERATURE] boiler %.2f\r\n", data.temp_boiler);
    }
  }
  
  if(deviceTemp.isNewValueAvailable()) {
    float newValue = deviceTemp.getTemperature();
    if(newValue > DEVICE_TEMPERATURE_LIMIT_MAX) {
      mainBoardError.set_error(DEVICE_OVERHEAT);
    } else {
      if(mainBoardError.has_error(DEVICE_OVERHEAT)) mainBoardError.clear_error(DEVICE_OVERHEAT);
      data.device_temperature = newValue;
      printf("[TEMPERATURE] internal %.2f\r\n", data.device_temperature);
    }
  }
}
