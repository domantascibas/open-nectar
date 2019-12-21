#ifndef __RELAY_STATE_H
#define __RELAY_STATE_H

#include <stdint.h>

typedef enum {
    SUN_RELAY,
    GRID_RELAY
} relayType_t;

typedef enum {
    OFF,
    ON,
    SWITCHING
} relayStateType_t;

typedef struct {
    relayStateType_t state;
    relayStateType_t state_next;
} relayControllerType_t;

void RelayState_init(void);
uint8_t RelayState_run(void);
uint8_t RelayState_set(relayType_t type, relayStateType_t state);
relayStateType_t RelayState_get(relayType_t type);

#endif
