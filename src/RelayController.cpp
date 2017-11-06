#include "RelayController.h"
#include "ErrorHandler.h"

GridRelay gridRelay;
SunRelay sunRelay;

RelayController::RelayController() {
  relayState = TURN_OFF_ALL;
}

void RelayController::init() {
  gridRelay.init();
  sunRelay.init();
  setRelays(TURN_OFF_ALL);
}

bool RelayController::isGridRelayOn() {
  return gridRelay.isOn();
}

bool RelayController::isSunRelayOn() {
  return sunRelay.isOn();
}

uint8_t RelayController::getRelayState() {
  return relayState;
}

void RelayController::setRelays(uint8_t state) {
  relayState = state;
  if(mainBoardError.has_error(DEVICE_OVERHEAT) || mainBoardError.has_error(MIN_TEMPERATURE) || mainBoardError.has_error(MAX_TEMPERATURE)) {
    relayState = TURN_OFF_ALL;
    printf("overheat/temperature-related errors\r\n");
  }
  switch(relayState) {
    default:
    case TURN_OFF_ALL:
      sunRelay.turnOff();
      gridRelay.turnOff();
      printf("turn off all\r\n");
      break;
    
    case TURN_ON_SUN:
      if(!powerBoardError.has_errors) {
        gridRelay.turnOff();
        sunRelay.turnOn();
        printf("turn on sun\r\n");
      } else {
        printf("power board error.\r\n");
        setRelays(TURN_OFF_ALL);
      }
      break;
    
    case TURN_ON_GRID:
      sunRelay.turnOff();
      gridRelay.turnOn();
      printf("turn on grid\r\n");
      break;
  }
}