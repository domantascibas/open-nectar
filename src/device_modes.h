#ifndef DEVICE_MODES_H
#define DEVICE_MODES_H

#include "mbed.h"
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

// *******************************Nectar Sun Copyright � Nectar Sun 2017*************************************   
