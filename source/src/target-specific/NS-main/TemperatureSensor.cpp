#include "TemperatureSensor.h"
#include "error_controller.h"
#include "consts.h"

TemperatureSensor::TemperatureSensor(PinName pin, uint8_t refresh)
: probe(pin), newValueAvailable(false), sensorFound(false), refreshRate(refresh) {
}

void TemperatureSensor::init() {
// 	temperature = 0.0;
// 	last_temperature = 0.0;
// 	errorCounter = 0;
//   if(probe.begin()) {
//     sensorFound = true;
//     measureTemperature();
//     attachTicker(refreshRate);
// 	}
// //    printf("Temperature sensor found\r\n");
// //  } else printf("Temperature sensor not found\r\n");
}

bool TemperatureSensor::isSensorFound() {
//   return sensorFound;
	return 1;
}

bool TemperatureSensor::isNewValueAvailable() {
//   return newValueAvailable;
	return 1;
}

float TemperatureSensor::getTemperature() {
//   newValueAvailable = false;
//   return temperature;
	return 85;
}

void TemperatureSensor::attachTicker(float interval) {
//   ticker.attach(callback(this, &TemperatureSensor::measureTemperature), interval);
}

void TemperatureSensor::detachTicker() {
//   ticker.detach();
}

void TemperatureSensor::measureTemperature() {
//   probe.startConversion();
//   timeout.attach(callback(this, &TemperatureSensor::readTemperatureToStorage), 0.75);
}

void TemperatureSensor::readTemperatureToStorage() {
	// if((identical_count >= LOST_SENSOR_COUNT) || mainBoardError.has_error(NO_BOILER_TEMP)) {
	// 	if(probe.begin()) {
	// 		sensorFound = true;
	// 		identical_count = 0;
	// 	} else {
	// 		sensorFound = false;
	// 		identical_count = LOST_SENSOR_COUNT;
	// 	}
	// }
	
	// if(sensorFound) {
	// 	last_temperature = temperature;
	// 	probe.read(temperature);
		
	// 	if(temperature == last_temperature) {
	// 		identical_count++;
	// 	} else {
	// 		identical_count = 0;
	// 	}
	
	// 	if(temperature != 85.00) {
	// 		if(mainBoardError.has_error(NO_BOILER_TEMP)) {
	// 			mainBoardError.clear_error(NO_BOILER_TEMP);
	// 		}
	// 		errorCounter = 0;
	// 		newValueAvailable = true;
	// 	} else {
	// 		errorCounter++;
	// 	}
		
	// 	if(errorCounter >= 10) {
	// 		errorCounter = 10;
	// 		temperature = 0.0;
	// 		mainBoardError.set_error(NO_BOILER_TEMP);
	// 		newValueAvailable = true;
	// 	}
	// } else {
	// 	temperature = 0.0;
	// 	mainBoardError.set_error(NO_BOILER_TEMP);
	// 	newValueAvailable = true;
	// }
}
