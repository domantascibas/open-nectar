#include "RelayController.h"
#include "error_controller.h"
#include "EnergyMeter.h"

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
	
  switch(relayState) {
    default:
    case TURN_OFF_ALL:
      sunRelay.turnOff();
      gridRelay.turnOff();
      gridMeter.stopMeter(gridRelay.isOn());
	    printf("turn off all\r\n");
    break;
		
    case TURN_OFF_SUN:
      sunRelay.turnOff();
      gridMeter.stopMeter(gridRelay.isOn());
      printf("turn off sun\r\n");
	  break;
    
    case TURN_ON_SUN:
      gridRelay.turnOff();
      sunRelay.turnOn();
      gridMeter.stopMeter(gridRelay.isOn());
      printf("turn on sun\r\n");
    break;
    
    case TURN_ON_GRID:
      sunRelay.turnOff();
      gridRelay.turnOn();
      gridMeter.startMeter();
      printf("turn on grid\r\n");
    break;
  }
}

bool RelayController::finishedSwitching() {
  if(!gridRelay.isSwitching() && !sunRelay.isSwitching()) return true;
  else return false;
}
