#include "Relay.h"

Relay::Relay(PinName pin) : relay(pin) {
  // turnedOn = false;
  // relayTurnOn = false;
  // switching = false;
}

void Relay::relayOn() {
  // turnedOn = true;
  // relay = turnedOn;
}

void Relay::relayOff() {
  // turnedOn = false;
  // relay = turnedOn;
}

bool Relay::isOn() {
  // return turnedOn;
}

bool Relay::isSwitching() {
  // return switching;
}
