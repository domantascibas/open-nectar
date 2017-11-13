#include "GridRelay.h"

static const PinName ZERO_CROSS = PB_5;
DigitalIn zeroCross(ZERO_CROSS);

void GridRelay::init() {
  printf("Grid relay init\r\n");
}

void GridRelay::turnOn() {
  relayTurnOn = true;
  switching = true;
  timeout.attach(callback(this, &GridRelay::waitForZeroCross), 1.0);
}

void GridRelay::turnOff() {
  relayTurnOn = false;
  switching = true;
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
  switching = false;
}