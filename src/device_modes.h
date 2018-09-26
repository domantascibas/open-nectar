#ifndef _NS_DEVICE_MODES_H
#define _NS_DEVICE_MODES_H

#include "MpptController.h"
#include "SensorController.h"

namespace device_modes {
  void setup();  
  void loop();
  void calibrate_device();
  void testingStandCalibrate();
  
  void start();
  void stop();
}

extern MpptController mppt;
extern SensorController sensors;

#endif
