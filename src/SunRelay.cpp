#include "SunRelay.h"
#include "PowerBoardComms.h"

void SunRelay::init() {
  t.start();
}

void SunRelay::turnOn() {
  delay(1);
  relayOn();
  delay(1);
  power_board::start();
}

void SunRelay::turnOff() {
  power_board::stop();
  delay(1);
  relayOff();
}

void SunRelay::delay(float sec) {
  t.reset();
  while(t.read() < sec) {}
}
