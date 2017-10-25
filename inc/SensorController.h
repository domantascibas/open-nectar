#ifndef SENSOR_CONTROLLER_H
#define SENSOR_CONTROLLER_H

#include "Sensor.h"

struct SensorController {
  SensorController();
  
  void init();
  void measure();
  void calibrate();
  float get_voltage_reference();
  float get_current_reference();
  void save_meters();
  
private:
  float get_voltage();
  float get_current();
};

#endif
