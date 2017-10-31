#ifndef TEMPERATURE_CONTROLLER_H
#define TEMPERATURE_CONTROLLER_H

#include "TemperatureSensor.h"

struct TemperatureController {
  TemperatureController() {};
  void init();
  void updateTemperatures();
  
  
};

#endif
