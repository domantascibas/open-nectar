#include "consts.h"
#include "TemperatureController.h"
#include "error_controller.h"
#include "DataService.h"
#include "device_modes.h"
#include "ServiceComms.h"
// #include "processor_temperature.h"

// TemperatureSensor boilerTemp(BOILER_TEMP_PIN, TEMPERATURE_SENSOR_UPDATE_PERIOD);

void TemperatureController::init() {
	// processor_temperature_init();
  // boilerTemp.init();
  
  // if(boilerTemp.isSensorFound()) {
  //   mainBoardError.clear_error(NO_BOILER_TEMP);
  // }
}

float TemperatureController::getBoilerTemperature() {
  // boilerTemperature = boilerTemp.getTemperature();
	// printf("TEMPERATURE BOILER %.2f\n", boilerTemperature);
  // if(boilerTemperature > WATER_TEMPERATURE_LIMIT_MAX) {
  //   mainBoardError.set_error(MAX_TEMPERATURE);
  // } else if(boilerTemperature < WATER_TEMPERATURE_LIMIT_MIN) {
  //   mainBoardError.set_error(MIN_TEMPERATURE);
  // } else {
  //   if(mainBoardError.has_error(MAX_TEMPERATURE)) mainBoardError.clear_error(MAX_TEMPERATURE);
  //   if(mainBoardError.has_error(MIN_TEMPERATURE)) mainBoardError.clear_error(MIN_TEMPERATURE);
  // }
  // return boilerTemperature;
  return 85;
}

void TemperatureController::updateTemperatures() {
//   if(boilerTemp.isNewValueAvailable() || service::isNewValueAvailable()) {
		
// 		float processor_temp = processor_temperature_measure();
// 		printf("TEMPERATURE PROCESSOR %.2f\n", processor_temp);
// 		if(processor_temp > PROCESSOR_INTERNAL_TEMPERATURE_LIMIT) {
// 			if(!mainBoardError.has_error(PROCESSOR_OVERHEAT)) mainBoardError.set_error(PROCESSOR_OVERHEAT);
// //			printf("TEMPERATURE OVERHEAT\n");
// 		} else {
// 			if(mainBoardError.has_error(PROCESSOR_OVERHEAT) && (processor_temp < (PROCESSOR_INTERNAL_TEMPERATURE_LIMIT - 5.0))) mainBoardError.clear_error(PROCESSOR_OVERHEAT);
// 		}
		
//     if(deviceOpMode.isInTestStand()) {
//       temperatureData.setBoilerTemperature(service::getFakeTemperature());
// //      printf("new fake temp\r\n");
//     } else {
//       temperatureData.setBoilerTemperature(getBoilerTemperature());
//     }
//     device_modes::updateHeaterMode = true;
//   }
}
