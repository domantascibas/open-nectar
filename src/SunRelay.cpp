#include "SunRelay.h"
#include "PowerBoardComms.h"

void SunRelay::init() {
  relayTurnOn = false;
  printf("Sun relay init\r\n");
}

void SunRelay::turnOn() {
  relayTurnOn = true;
  timeout.attach(callback(this, &SunRelay::timeoutIsr), 2.0);
  power_board::start();
}

void SunRelay::turnOff() {
  relayTurnOn = false;
  power_board::stop();
  timeout.attach(callback(this, &SunRelay::timeoutIsr), 1.0);
}

void SunRelay::timeoutIsr() {
  if(relayTurnOn) {
    relayOn();
  } else {
    relayOff();
  }
}
