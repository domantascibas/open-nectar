#ifndef GRID_RELAY_H
#define GRID_RELAY_H

#include "Relay.h"
static const PinName GRID_PIN = PB_4;

struct GridRelay : Relay {
  GridRelay() : Relay(GRID_PIN) {};
  void init();
    
  void turnOn();
  void turnOff();
  
  private:
  void timeoutIsr();
  void zeroCrossHigh();
  void waitForZeroCross();
  Timer t;
};

#endif
