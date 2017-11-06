#ifndef DEVICE_MODES_H
#define DEVICE_MODES_H

#include "RelayController.h"

namespace device_modes {
  void setup();
  void loop();
  void reset();
  bool isGridRelayOn();
  bool isSunRelayOn();
}

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
