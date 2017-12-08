#include "TemperatureSensor.h"
#include "data.h"

TemperatureSensor::TemperatureSensor(PinName pin, uint8_t refresh)
: probe(pin), temperature(0.0), newValueAvailable(false), sensorFound(false), refreshRate(refresh) {
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

float TemperatureSensor::getTemperature() {
  newValueAvailable = false;
  return temperature;
}

void TemperatureSensor::attachTicker(float interval) {
  printf("[TEMPERATURE] %.2fs ticker attached\r\n", interval);
  ticker.attach(callback(this, &TemperatureSensor::measureTemperature), interval);
}

void TemperatureSensor::detachTicker() {
  ticker.detach();
}

void TemperatureSensor::measureTemperature() {
  probe.startConversion();
  timeout.attach(callback(this, &TemperatureSensor::readTemperatureToStorage), 2.0);
}

void TemperatureSensor::readTemperatureToStorage() {
  if(!data.readingSerial) {
    probe.read(temperature);
    newValueAvailable = true;
  } else timeout.attach(callback(this, &TemperatureSensor::readTemperatureToStorage), 2.0);
}
