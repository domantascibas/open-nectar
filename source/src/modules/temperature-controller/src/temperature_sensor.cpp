#include "consts.h"
#include "temperature_sensor.h"
// #include "data.h"

extern "C" {
    #include "data.h"
}

static const float TIMEOUT_TO_TEMP_READ = 0.5;

TemperatureSensor::TemperatureSensor(PinName pin, uint8_t refresh)
: probe(pin) {
  temperature = 0.0;
  newValueAvailable = false;
  sensorFound = false;
  refreshRate = refresh;
  readyToMeasure = false;
  readyToRead = false;
}

void TemperatureSensor::init(void) {
  if(probe.begin()) {
    printf("[TEMPERATURE] sensor found\r\n");
    sensorFound = true;
    measureTemperature();
    attachTicker(refreshRate);
  } else printf("[TEMPERATURE] sensor NOT found\r\n");
}

bool TemperatureSensor::isSensorFound(void) {
  return sensorFound;
}

bool TemperatureSensor::isNewValueAvailable(void) {
  return newValueAvailable;
}

void TemperatureSensor::setNewValueNotAvailable(void) {
  newValueAvailable = false;
}

float TemperatureSensor::getTemperature(void) {
  newValueAvailable = false;
  return temperature;
}

void TemperatureSensor::attachTicker(float interval) {
  printf("[TEMPERATURE] %.2fs ticker attached\r\n", interval);
  ticker.attach(callback(this, &TemperatureSensor::setReadyToMeasure), interval);
}

void TemperatureSensor::detachTicker(void) {
  ticker.detach();
}

void TemperatureSensor::setReadyToMeasure(void) {
  readyToMeasure = true;
}

void TemperatureSensor::setReadyToRead(void) {
  readyToRead = true;
}

bool TemperatureSensor::isReadyToMeasure(void) {
  return readyToMeasure;
}

bool TemperatureSensor::isReadyToRead(void) {
  return readyToRead;
}

void TemperatureSensor::measureTemperature(void) {
  readyToMeasure = false;
  probe.startConversion();
  timeout.attach(callback(this, &TemperatureSensor::setReadyToRead), TIMEOUT_TO_TEMP_READ);
}

void TemperatureSensor::readTemperatureToStorage(void) {
  readyToRead = false;
  temperature = probe.read();
  newValueAvailable = true;
}
