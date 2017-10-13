#ifndef STATE_SERVICE_H
#define STATE_SERVICE_H
#include "data.h"

typedef enum {
  WELCOME,
  NOT_CONFIGURED,
  CONFIGURED,
  TEST_MODE
} DeviceState;

namespace StateService {
  extern DeviceState currentDeviceState;
  
  float get_temperature();
  float get_temperature_max();
  float get_boiler_power();
  HeaterMode *get_device_mode();
  DeviceState *get_state();
  
  void set_state(DeviceState);
}

#endif
