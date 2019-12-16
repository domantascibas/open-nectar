#include "main-controller.h"
#include "data.h"
#include <stdio.h>
#include "generator-controller.h"

static powerStateDataType_t state;
static void stopMppt(void);

void MainController_init(void) {
    state = IDLE_STATE;
}

powerStateDataType_t MainController_run(void) {
    switch(state) {
        case IDLE_STATE:
            if(GET_STATUS(SUN_STATUS) && GET_STATUS(V_READY_STATUS)) {
                if(generatorController_run()) {
                    // delay(200ms)
                    state = PWM_IDLE_STATE;
                }
            } else {
                state = IDLE;
            }
        break;

        case PWM_IDLE_STATE:
            if(GET_STATUS(SUN_STATUS) && GET_STATUS(V_READY_STATUS)) {
                if(GET_STATUS(I_READY_STATUS)) {
                    if(pwmController_run()) {
                        // delay(200ms)
                        state = MPPT_RUN_STATE;
                    }
                } else {
                    state = PWM_IDLE_STATE;
                }
            } else {
                state = RETURN_TO_IDLE;
            }
        break;

        case MPPT_RUN_STATE:
            if(GET_STATUS(SUN_STATUS) && GET_STATUS(V_READY_STATUS)) {

            } else {
                state = RETURN_TO_IDLE;
            }
        break;

        case RETURN_TO_IDLE:
            stopMppt();
            state = IDLE_STATE;
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

static void stopMppt(void) {
    // stop Mppt, delay, stop pwm, delay, turn off SD
    // delay(200ms)
    pwmController_stop();
    // delay(200ms)
    generatorController_stop();
}
