#ifndef RELAY_H
#define RELAY_H

#include "mbed.h"

struct Relay {
  Relay(PinName pin) : turnedOn(false), relay(pin) {};  
  void relayOn();
  void relayOff();
  bool isOn();
  
  virtual void turnOn() = 0;
  virtual void turnOff() = 0;
    
  private:
  bool turnedOn;
  DigitalOut relay;
};

#endif
