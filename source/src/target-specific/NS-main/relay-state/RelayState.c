#include "RelayState.h"

#define TRUE 1
#define FALSE 0

relayControllerType_t sunRelayNew = {OFF, OFF};
relayControllerType_t gridRelayNew = {OFF, OFF};

uint8_t is_switching(void);

void RelayState_init(void) {
    sunRelayNew.state = OFF;
    sunRelayNew.state_next = OFF;

    gridRelayNew.state = OFF;
    gridRelayNew.state_next = OFF;
}

uint8_t RelayState_run(void) {
    if (!is_switching()) {
        // check that both relays can't turn on at the same time
        if (sunRelayNew.state != sunRelayNew.state_next) {
            if (sunRelayNew.state != SWITCHING) {
                sunRelayNew.state = SWITCHING;
                // set timeout
            } else {
                sunRelayNew.state = sunRelayNew.state_next;
            }
        }

        if (gridRelayNew.state != gridRelayNew.state_next) {
            if (gridRelayNew.state != SWITCHING) {
                gridRelayNew.state = SWITCHING;
                // set timeout
            } else {
                gridRelayNew.state = gridRelayNew.state_next;
            }
        }
    }
    return 1;
}

uint8_t RelayState_set(relayType_t type, relayStateType_t state) {
    switch (type) {
        case SUN_RELAY:
            sunRelayNew.state_next = state;
            break;

        case GRID_RELAY:
            gridRelayNew.state_next = state;
            break;

        default:
            break;
    }
    return 1;
}

relayStateType_t RelayState_get(relayType_t type) {
    switch (type) {
        case SUN_RELAY:
            return sunRelayNew.state;
            break;

        case GRID_RELAY:
            return gridRelayNew.state;
            break;

        default:
            break;
    }
    return 3;
}

uint8_t is_switching(void) {
    if (sunRelayNew.state == SWITCHING || gridRelayNew.state == SWITCHING) {
        return 1;
    }
    return 0;
}
