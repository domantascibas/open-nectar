#ifndef RELAY_H
#define RELAY_H

#include "mbed.h"

struct Relay {
  Relay(PinName pin); 
  void relayOn();
  void relayOff();
  bool isOn();
  
  virtual void turnOn() = 0;
  virtual void turnOff() = 0;
  bool relayTurnOn;
  Timeout timeout;
  bool isSwitching();
  bool switching;
    
  private:
  bool turnedOn;
  DigitalOut relay;
};

#endif
