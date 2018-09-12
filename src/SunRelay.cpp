#include "SunRelay.h"
#include "PowerBoardComms.h"

void SunRelay::init() {
  printf("Sun relay init\r\n");
}

void SunRelay::turnOn() {
  relayTurnOn = true;
  switching = true;
  timeout.attach(callback(this, &SunRelay::timeoutIsr), 3.0);
}

void SunRelay::turnOff() {
  relayTurnOn = false;
  switching = true;
  timeout.attach(callback(this, &SunRelay::timeoutSd), 0.5);
}

void SunRelay::timeoutSd() {
	if(relayTurnOn) {
		power_board::start();
		switching = false;
  } else {
		power_board::stop();
    timeout.attach(callback(this, &SunRelay::timeoutIsr), 3.0);
  }
}

void SunRelay::timeoutIsr() {
  if(relayTurnOn) {
		relayOn();
		timeout.attach(callback(this, &SunRelay::timeoutSd), 1.0);
  } else {
    relayOff();
		switching = false;
  }
}
