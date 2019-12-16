#include "main-controller.h"
#include "data.h"
#include <stdio.h>
#include "generator-controller.h"

typedef struct powerState_tag {
    powerStateDataType_t curr_state;
    powerStateDataType_t next_state;
} powerState_t;

static const powerState_t pIdle_state =      {IDLE_STATE,        PWM_ON_STATE};
static const powerState_t pPwmOn_state =     {PWM_ON_STATE,      MPPT_RUN_STATE};
// static const powerState_t pMpptRun_state =   {MPPT_RUN_STATE,    IDLE_STATE};

powerState_t current_state;

void MainController_init(void) {
    current_state = pIdle_state;
}

powerStateDataType_t MainController_run(void) {
    switch(current_state.curr_state) {
        case IDLE_STATE:
            if(GET_STATUS(SUN_STATUS) && GET_STATUS(V_READY_STATUS)) {
                if(generatorController_run() && pwmController_run()) {
                    // delay(200ms)
                    current_state = pPwmOn_state;
                }
            }
        break;

        case PWM_ON_STATE:
            if(GET_STATUS(SUN_STATUS) && GET_STATUS(I_READY_STATUS)) {
                
            }
        break;

        case MPPT_RUN_STATE:
        break;

        case POWER_STATE_COUNT:
        default:
        break;
    }

    return current_state.curr_state;
}

powerStateDataType_t MainController_current_state_get(void) {
    return current_state.curr_state;
}
