#ifndef _NS_RELAY_CONTROLLER_H
#define _NS_RELAY_CONTROLLER_H

#include "GridRelay.h"
#include "SunRelay.h"

typedef enum {
    TURN_OFF_ALL,
    TURN_ON_SUN,
    TURN_ON_GRID,
    TURN_OFF_SUN
} RelayStates;

struct RelayController {
    RelayController();

    void init();
    void setRelays(uint8_t);
    uint8_t getRelayState();
    bool isSunRelayOn();
    bool isGridRelayOn();
    bool finishedSwitching();

private:
    uint8_t relayState;
};

#endif
