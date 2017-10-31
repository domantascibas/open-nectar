#ifndef RELAY_CONTROLLER_H
#define RELAY_CONTROLLER_H

#include "GridRelay.h"
#include "SunRelay.h"

enum RelayStates {
  TURN_OFF_ALL,
  TURN_ON_SUN,
  TURN_ON_GRID
};

struct RelayController {
  RelayController();
  
  void init();
  void setRelays(uint8_t);
  uint8_t getRelayState();
  bool isSunRelayOn();
  bool isGridRelayOn();
  
  private:
  uint8_t relayState;
};

#endif
