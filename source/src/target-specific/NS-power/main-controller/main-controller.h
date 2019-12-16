#ifndef POWER_MAIN_CONTROLLER_H
#define POWER_MAIN_CONTROLLER_H

typedef enum {
    IDLE_STATE,
    PWM_ON_STATE,
    MPPT_RUN_STATE,
    POWER_STATE_COUNT
} powerStateDataType_t;

void MainController_init(void);
powerStateDataType_t MainController_run(void);
powerStateDataType_t MainController_state_get(void);

#endif
