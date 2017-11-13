#include "SunRelay.h"
#include "PowerBoardComms.h"

void SunRelay::init() {
  printf("Sun relay init\r\n");
}

void SunRelay::turnOn() {
  relayTurnOn = true;
  switching = true;
  timeout.attach(callback(this, &SunRelay::timeoutIsr), 1.0);
  power_board::start();
}

void SunRelay::turnOff() {
  relayTurnOn = false;
  switching = true;
  power_board::stop();
  timeout.attach(callback(this, &SunRelay::timeoutIsr), 0.5);
}

void SunRelay::timeoutIsr() {
  if(relayTurnOn) {
    relayOn();
  } else {
    relayOff();
  }
  switching = false;
}
