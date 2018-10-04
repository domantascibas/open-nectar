#include "consts.h"
#include "TemperatureController.h"
#include "ErrorHandler.h"
#include "DataService.h"
#include "device_modes.h"
#include "ServiceComms.h"
#include "processor_temperature.h"

TemperatureSensor boilerTemp(BOILER_TEMP_PIN, 3);

void TemperatureController::init() {
	processor_temperature_init();
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
		
		float processor_temp = processor_temperature_measure();
		// printf("processor temp: %f\r\n", internal_temp);
		if(processor_temp > PROCESSOR_INTERNAL_TEMPERATURE_LIMIT) {
			if(!mainBoardError.has_error(PROCESSOR_OVERHEAT)) mainBoardError.set_error(PROCESSOR_OVERHEAT);
			printf("PROCESSOR OVERHEAT\r\n");
		} else {
			if(mainBoardError.has_error(PROCESSOR_OVERHEAT) && (processor_temp < (PROCESSOR_INTERNAL_TEMPERATURE_LIMIT - 5.0))) mainBoardError.clear_error(PROCESSOR_OVERHEAT);
		}
		
    if(deviceOpMode.isInTestStand()) {
      temperatureData.setBoilerTemperature(service::getFakeTemperature());
      printf("new fake temp\r\n");
    } else {
      temperatureData.setBoilerTemperature(getBoilerTemperature());
    }
    device_modes::updateHeaterMode = true;
  }
}
