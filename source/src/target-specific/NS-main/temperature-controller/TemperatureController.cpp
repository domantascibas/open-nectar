#include "consts.h"
#include "TemperatureController.h"
#include "TemperatureSensor.h"
#include "processor_temperature.h"
#include "error_handler.h"
#include "ServiceComms.h"
#include "DeviceMode.h"
#include "device_modes.h"
#include "DataService.h"

uint8_t boilerTemperature;

void temperatureController_init(void) {
  boilerTemperature = 0;

  processor_temperature_init();
  temperatureSensor_init();

  if (temperatureSensor_isFound()) {
    // mainBoardError.clear_error(NO_BOILER_TEMP);
    error_clear(NS_NO_BOILER_TEMP);
  }
}

uint8_t temperatureController_getBoilerTemp(void) {
  boilerTemperature = temperatureSensor_get();
	printf("TEMPERATURE BOILER %d\n", boilerTemperature);
  if(boilerTemperature > WATER_TEMPERATURE_LIMIT_MAX) {
    // mainBoardError.set_error(MAX_TEMPERATURE);
    error_set(NS_MAX_TEMPERATURE);
  } else if (boilerTemperature < WATER_TEMPERATURE_LIMIT_MIN) {
    // mainBoardError.set_error(MIN_TEMPERATURE);
    error_set(NS_MIN_TEMPERATURE);
  } else {
    // if (mainBoardError.has_error(MAX_TEMPERATURE)) mainBoardError.clear_error(MAX_TEMPERATURE);
    // if (mainBoardError.has_error(MIN_TEMPERATURE)) mainBoardError.clear_error(MIN_TEMPERATURE);
      if (error_isSet(NS_MAX_TEMPERATURE)) error_clear(NS_MAX_TEMPERATURE);
      if (error_isSet(NS_MIN_TEMPERATURE)) error_clear(NS_MIN_TEMPERATURE);
  }
  return boilerTemperature;
}

void temperatureController_update(void) {
  if (temperatureSensor_isNewValAvail() || service_newValAvail()) {
		float processor_temp = processor_temperature_measure();
		printf("TEMPERATURE PROCESSOR %.2f\n", processor_temp);
		if (processor_temp > PROCESSOR_INTERNAL_TEMPERATURE_LIMIT) {
			// if (!mainBoardError.has_error(PROCESSOR_OVERHEAT)) mainBoardError.set_error(PROCESSOR_OVERHEAT);
            if (!error_isSet(NS_PROCESSOR_OVERHEAT_MAIN)) error_set(NS_PROCESSOR_OVERHEAT_MAIN);
//			printf("TEMPERATURE OVERHEAT\n");
		} else {
			// if (mainBoardError.has_error(PROCESSOR_OVERHEAT) && (processor_temp < (PROCESSOR_INTERNAL_TEMPERATURE_LIMIT - 5.0))) mainBoardError.clear_error(PROCESSOR_OVERHEAT);
            if (error_isSet(NS_PROCESSOR_OVERHEAT_MAIN) && (processor_temp < (PROCESSOR_INTERNAL_TEMPERATURE_LIMIT - 5))) error_clear(NS_PROCESSOR_OVERHEAT_MAIN);
		}
		
    if (deviceOpMode_isInTestStand()) {
      temperatureData_setBoilerTemperature(service_getFakeTemperature());
     printf("new fake temp\r\n");
    } else {
      temperatureData_setBoilerTemperature(temperatureController_getBoilerTemp());
    }
    device_modes_setHeaterMode(1);
  }
}
