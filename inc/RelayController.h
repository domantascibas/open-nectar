#ifndef RELAY_CONTROLLER_H
#define RELAY_CONTROLLER_H

#include "GridRelay.h"
#include "SunRelay.h"

struct RelayController {
  RelayController();
  
  void turnOnGrid();
  void turnOnSun();
  void turnOffAll();
  
  bool isSunRelayOn();
  bool isGridRelayOn();
};

#endif
