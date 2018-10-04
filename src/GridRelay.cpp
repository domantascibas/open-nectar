#include "consts.h"
#include "GridRelay.h"

DigitalIn zeroCross(ZERO_CROSS_PIN);

void GridRelay::init() {
  printf("Grid relay init\r\n");
}

void GridRelay::turnOn() {
  relayTurnOn = true;
  switching = true;
  timeout.attach(callback(this, &GridRelay::waitForZeroCross), 5.0);
}

void GridRelay::turnOff() {
  relayTurnOn = false;
  switching = true;
  timeout.attach(callback(this, &GridRelay::waitForZeroCross), 1.0);
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