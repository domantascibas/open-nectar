#include "TemperatureSensor.h"
#include "ErrorHandler.h"

TemperatureSensor::TemperatureSensor(PinName pin, uint8_t refresh)
: probe(pin), newValueAvailable(false), sensorFound(false), refreshRate(refresh) {
}

void TemperatureSensor::init() {
	temperature = 0.0;
	errorCounter = 0;
  if(probe.begin()) {
    sensorFound = true;
    measureTemperature();
    attachTicker(refreshRate);
    printf("Temperature sensor found\r\n");
  } else printf("Temperature sensor not found\r\n");
}

bool TemperatureSensor::isSensorFound() {
  return sensorFound;
}

bool TemperatureSensor::isNewValueAvailable() {
  return newValueAvailable;
}

float TemperatureSensor::getTemperature() {
  newValueAvailable = false;
  return temperature;
}

void TemperatureSensor::attachTicker(float interval) {
  ticker.attach(callback(this, &TemperatureSensor::measureTemperature), interval);
}

void TemperatureSensor::detachTicker() {
  ticker.detach();
}

void TemperatureSensor::measureTemperature() {
  probe.startConversion();
  timeout.attach(callback(this, &TemperatureSensor::readTemperatureToStorage), 0.75);
}

void TemperatureSensor::readTemperatureToStorage() {
  probe.read(temperature);
	if(temperature != 85.00) {
		if(mainBoardError.has_error(NO_BOILER_TEMP)) {
			mainBoardError.clear_error(NO_BOILER_TEMP);
		}
		errorCounter = 0;
		newValueAvailable = true;
	} else {
		errorCounter++;
	}
	
	if(errorCounter >= 10) {
		errorCounter = 10;
		temperature = 0.0;
		mainBoardError.set_error(NO_BOILER_TEMP);
	}
}
