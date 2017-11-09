#include "GridRelay.h"

static const PinName ZERO_CROSS = PB_5;
DigitalIn zeroCross(ZERO_CROSS);

void GridRelay::init() {
  relayTurnOn = false;
  printf("Grid relay init\r\n");
}

void GridRelay::turnOn() {
  relayTurnOn = true;
  timeout.attach(callback(this, &GridRelay::waitForZeroCross), 2.0);
}

void GridRelay::turnOff() {
  relayTurnOn = false;
  timeout.attach_us(callback(this, &GridRelay::waitForZeroCross), 500);
}

void GridRelay::waitForZeroCross() {
  while(!zeroCross) {};
  timeout.attach_us(callback(this, &GridRelay::timeoutIsr), 600);
}

void GridRelay::timeoutIsr() {
  if(relayTurnOn) {
    relayOn();
  } else {
    relayOff();
  }
}