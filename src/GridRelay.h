#ifndef GRID_RELAY_H
#define GRID_RELAY_H

#include "consts.h"
#include "Relay.h"

struct GridRelay : Relay {
  GridRelay() : Relay(GRID_RELAY_PIN) {};
  void init();
    
  virtual void turnOn();
  virtual void turnOff();
  
  private:
  void timeoutIsr();
  void zeroCrossHigh();
  void waitForZeroCross();
  Timer t;
};

#endif
