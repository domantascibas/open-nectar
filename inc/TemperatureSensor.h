#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include "mbed.h"
#include "DS1820.h"

struct TemperatureSensor {
  TemperatureSensor(PinName);
  void init();
  bool isSensorFound();
  bool newValueAvailable;
  bool isNewValueAvailable();
  float getTemperature();
  
private:
  DS1820 probe;
  Ticker ticker;
  Timeout timeout;
  float temperature;
  bool sensorFound;
  void attachTicker(float);
  void detachTicker();
  void measureTemperature();
  void readTemperatureToStorage();
};

#endif
