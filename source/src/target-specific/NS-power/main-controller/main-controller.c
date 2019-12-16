#include "main-controller.h"
#include "data.h"
#include <stdio.h>
#include "generator-controller.h"

static powerStateDataType_t state;

void MainController_init(void) {
    state = IDLE_STATE;
}

powerStateDataType_t MainController_run(void) {
    switch(state) {
        case IDLE_STATE:
            if(GET_STATUS(SUN_STATUS) && GET_STATUS(V_READY_STATUS)) {
                if(generatorController_run()) {
                    // delay(200ms)
                    state = PWM_ON_STATE;
                }
            }
        break;

        case PWM_ON_STATE:
        break;

        case MPPT_RUN_STATE:
        break;

        case POWER_STATE_COUNT:
        default:
        break;
    }

    return state;
}

powerStateDataType_t MainController_state_get(void) {
    return state;
}
