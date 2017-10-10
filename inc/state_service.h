#ifndef STATE_SERVICE_H
#define STATE_SERVICE_H
#include "data.h"

namespace StateService {  
  void setup();
  void loop();
  
  float get_temperature();
  float get_temperature_max();
  float get_boiler_power();
  HeaterMode *get_device_mode();
}

#endif
