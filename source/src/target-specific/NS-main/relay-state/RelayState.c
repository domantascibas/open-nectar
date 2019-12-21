#include "RelayState.h"

#define TRUE 1
#define FALSE 0

relayControllerType_t sunRelay = {OFF, OFF};
relayControllerType_t gridRelay = {OFF, OFF};

uint8_t is_switching(void);

void RelayState_init(void) {
    sunRelay.state = OFF;
    sunRelay.state_next = OFF;

    gridRelay.state = OFF;
    gridRelay.state_next = OFF;
}

uint8_t RelayState_run(void) {
    if (!is_switching()) {
        // check that both relays can't turn on at the same time
        if (sunRelay.state != sunRelay.state_next) {
            if (sunRelay.state != SWITCHING) {
                sunRelay.state = SWITCHING;
                // set timeout
            } else {
                sunRelay.state = sunRelay.state_next;
            }
        }

        if (gridRelay.state != gridRelay.state_next) {
            if (gridRelay.state != SWITCHING) {
                gridRelay.state = SWITCHING;
                // set timeout
            } else {
                gridRelay.state = gridRelay.state_next;
            }
        }
    }
    return 1;
}

uint8_t RelayState_set(relayType_t type, relayStateType_t state) {
    switch (type) {
        case SUN_RELAY:
            sunRelay.state_next = state;
            break;

        case GRID_RELAY:
            gridRelay.state_next = state;
            break;

        default:
            break;
    }
    return 1;
}

relayStateType_t RelayState_get(relayType_t type) {
    switch (type) {
        case SUN_RELAY:
            return sunRelay.state;
            break;

        case GRID_RELAY:
            return gridRelay.state;
            break;

        default:
            break;
    }
    return 3;
}

uint8_t is_switching(void) {
    if (sunRelay.state == SWITCHING || gridRelay.state == SWITCHING) {
        return 1;
    }
    return 0;
}
