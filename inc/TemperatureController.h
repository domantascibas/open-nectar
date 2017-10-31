#ifndef TEMPERATURE_CONTROLLER_H
#define TEMPERATURE_CONTROLLER_H

#include "TemperatureSensor.h"

struct TemperatureController {
  TemperatureController() : boilerTemperature(0.0), deviceTemperature(0.0) {};
  void init();
  float getBoilerTemperature();
  float getDeviceTemperature();
  void updateTemperatures(float*, float*);
    
private:
  float boilerTemperature;
  float deviceTemperature;
};

#endif
