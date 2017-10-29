#include "GridRelay.h"

static const PinName ZERO_CROSS = PB_5;
DigitalIn zeroCross(ZERO_CROSS);

void GridRelay::init() {
  t.start();
}

void GridRelay::turnOn() {
  delay(1);
  waitForZeroCross();
  relayOn();
}

void GridRelay::turnOff() {
  waitForZeroCross();
  relayOff();
}

void GridRelay::waitForZeroCross() {
  t.reset();
  while(!zeroCross) {}
  while(t.read_us() < 600) {}
}

void GridRelay::delay(float sec) {
  t.reset();
  while(t.read() < sec) {}
}