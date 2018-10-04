#ifndef _NS_TEMPERATURE_CONTROLLER_H
#define _NS_TEMPERATURE_CONTROLLER_H

#include "TemperatureSensor.h"

struct TemperatureController {
  TemperatureController() : boilerTemperature(0.0) {};
  void init();
  float getBoilerTemperature();
  void updateTemperatures();
    
private:
  float boilerTemperature;
};

#endif
