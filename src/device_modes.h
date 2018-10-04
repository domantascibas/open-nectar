#ifndef _NS_DEVICE_MODES_H
#define _NS_DEVICE_MODES_H

#include "RelayController.h"

namespace device_modes {
  extern bool updateHeaterMode;
  void setup();
  void loop();
  void reset();
  bool isGridRelayOn();
  bool isSunRelayOn();
	bool turnOffRelays();
}

#endif
