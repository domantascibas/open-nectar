#ifndef _NS_TEMPERATURE_SENSOR_H
#define _NS_TEMPERATURE_SENSOR_H

#include "mbed.h"
#include "DS1820.h"

struct TemperatureSensor {
  TemperatureSensor(PinName, uint8_t);
  void init();
  bool isSensorFound();
  bool newValueAvailable;
  bool isNewValueAvailable();
  float getTemperature();
  
private:
  DS1820 probe;
  Ticker ticker;
  Timeout timeout;
	uint8_t errorCounter;
  uint8_t refreshRate;
  float temperature;
  bool sensorFound;
  void attachTicker(float);
  void detachTicker();
  void measureTemperature();
  void readTemperatureToStorage();
};

#endif
