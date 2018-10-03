#ifndef _NS_TEMPERATURE_SENSOR_H
#define _NS_TEMPERATURE_SENSOR_H

#include "consts.h"
#include "DS1820.h"

struct TemperatureSensor {
  TemperatureSensor(PinName, uint8_t);
  void init(void);
  bool isSensorFound(void);
  bool isNewValueAvailable(void);
  bool isReadyToRead(void);
  bool isReadyToMeasure(void);
  void setNewValueNotAvailable(void);
  void measureTemperature(void);
  void readTemperatureToStorage(void);
  float getTemperature(void);

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
  void detachTicker(void);
  void setReadyToRead(void);
  void setReadyToMeasure(void);
};

#endif