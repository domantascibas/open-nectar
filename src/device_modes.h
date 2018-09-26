#ifndef _NS_DEVICE_MODES_H
#define _NS_DEVICE_MODES_H

#include "SensorController.h"

namespace device_modes {
  void setup();  
  void loop();
  void calibrate_device();
  void testingStandCalibrate();
  
  void start();
  void stop();
}

extern SensorController sensors;

#endif
