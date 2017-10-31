#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(PinName pin)
: probe(pin), temperature(0.0), newValueAvailable(false), sensorFound(false) {
}

void TemperatureSensor::init() {
  if(probe.begin()) {
    sensorFound = true;
    measureTemperature();
    attachTicker(10);
  }
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
  timeout.attach(callback(this, &TemperatureSensor::readTemperatureToStorage), 2.0);
}

void TemperatureSensor::readTemperatureToStorage() {
  probe.read(temperature);
  newValueAvailable = true;
}
