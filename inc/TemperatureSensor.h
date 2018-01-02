#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include "mbed.h"
#include "DS1820.h"

struct TemperatureSensor {
  TemperatureSensor(PinName, uint8_t);
  void init();
  bool isSensorFound();
  bool isNewValueAvailable();
  bool isReadyToRead();
  bool isReadyToMeasure();
  void setNewValueNotAvailable();
  void measureTemperature();
  void readTemperatureToStorage();
  float getTemperature();
  
private:
  DS1820 probe;
  Ticker ticker;
  Timeout timeout;
  uint8_t refreshRate;
  float temperature;
  bool sensorFound;
  bool newValueAvailable;
  bool readyToMeasure;
  bool readyToRead;
  void attachTicker(float);
  void detachTicker();
  void setReadyToRead();
  void setReadyToMeasure();
};

#endif
