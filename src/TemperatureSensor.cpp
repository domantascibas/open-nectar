#include "TemperatureSensor.h"
#include "data.h"

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

void TemperatureSensor::init() {
  if(probe.begin()) {
    printf("[TEMPERARTURE] sensor found\r\n");
    sensorFound = true;
    measureTemperature();
    attachTicker(refreshRate);
  } else printf("[TEMPERARTURE] sensor NOT found\r\n");
}

bool TemperatureSensor::isSensorFound() {
  return sensorFound;
}

bool TemperatureSensor::isNewValueAvailable() {
  return newValueAvailable;
}

void TemperatureSensor::setNewValueNotAvailable() {
  newValueAvailable = false;
}

float TemperatureSensor::getTemperature() {
//  printf("[TEMP PROBE] returning temp\r\n");
  newValueAvailable = false;
  return temperature;
}

void TemperatureSensor::attachTicker(float interval) {
  printf("[TEMPERATURE] %.2fs ticker attached\r\n", interval);
  ticker.attach(callback(this, &TemperatureSensor::setReadyToMeasure), interval);
}

void TemperatureSensor::detachTicker() {
  ticker.detach();
}

void TemperatureSensor::setReadyToMeasure() {
  readyToMeasure = true;
}

void TemperatureSensor::setReadyToRead() {
  readyToRead = true;
}

bool TemperatureSensor::isReadyToMeasure() {
  return readyToMeasure;
}

bool TemperatureSensor::isReadyToRead() {
  return readyToRead;
}

void TemperatureSensor::measureTemperature() {
  readyToMeasure = false;
//  printf("[TEMP PROBE] started conversion\r\n");
  probe.startConversion();
  timeout.attach(callback(this, &TemperatureSensor::setReadyToRead), TIMEOUT_TO_TEMP_READ);
//  printf("[TEMP PROBE] conversion end\r\n");
}

void TemperatureSensor::readTemperatureToStorage() {
  readyToRead = false;
//  printf("[TEMP PROBE] reading temp to storage\r\n");
  temperature = probe.read();
  newValueAvailable = true;
//  printf("[TEMP PROBE] new temp available\r\n");
}
