#ifndef SUN_RELAY_H
#define SUN_RELAY_H

#include "Relay.h"
static const PinName SUN_PIN = PB_3;

struct SunRelay : Relay {
  SunRelay() : Relay(SUN_PIN) {};
  void init();
  virtual void turnOn();
  virtual void turnOff();
    
  private:
  void timeoutIsr();
};

#endif
