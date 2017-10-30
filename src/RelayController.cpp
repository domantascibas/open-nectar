#include "RelayController.h"
#include "ErrorHandler.h"
#include "data.h"

GridRelay gridRelay;
SunRelay sunRelay;

RelayController::RelayController() {
  gridRelay.init();
  sunRelay.init();
}

void RelayController::turnOffAll() {
  sunRelay.turnOff();
  gridRelay.turnOff();
  data.grid_relay_on = gridRelay.isOn();
  data.sun_relay_on = sunRelay.isOn();
  printf("turn off all\r\n");
}

void RelayController::turnOnGrid() {
  sunRelay.turnOff();
  gridRelay.turnOn();
  data.grid_relay_on = gridRelay.isOn();
  data.sun_relay_on = sunRelay.isOn();
  printf("turn on grid\r\n");
}

void RelayController::turnOnSun() {
  if(!powerBoardError.has_errors) {
    gridRelay.turnOff();
    sunRelay.turnOn();
    data.grid_relay_on = gridRelay.isOn();
    data.sun_relay_on = sunRelay.isOn();
    printf("turn on sun\r\n");
  } else {
    turnOffAll();
  }
}

bool RelayController::isGridRelayOn() {
  return gridRelay.isOn();
}

bool RelayController::isSunRelayOn() {
  return sunRelay.isOn();
}