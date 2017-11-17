#include "TemperatureController.h"
#include "ErrorHandler.h"
#include "DataService.h"
#include "device_modes.h"
#include "ServiceComms.h"

static const PinName BOILER_TEMP_PROBE = PB_8;

static const float WATER_TEMPERATURE_LIMIT_MIN = 5.0;
static const float WATER_TEMPERATURE_LIMIT_MAX = 90.0;

TemperatureSensor boilerTemp(BOILER_TEMP_PROBE, 3);

void TemperatureController::init() {
  boilerTemp.init();
  
  if(boilerTemp.isSensorFound()) {
    mainBoardError.clear_error(NO_BOILER_TEMP);
  }
}

float TemperatureController::getBoilerTemperature() {
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
  return boilerTemperature;
}


void TemperatureController::updateTemperatures() {
  if(boilerTemp.isNewValueAvailable() || service::isNewValueAvailable()) {
    if(deviceOpMode.isInTestStand()) {
      temperatureData.setBoilerTemperature(service::getFakeTemperature());
      printf("new fake temp\r\n");
    } else {
      temperatureData.setBoilerTemperature(getBoilerTemperature());
    }
    device_modes::updateHeaterMode = true;
  }
}
