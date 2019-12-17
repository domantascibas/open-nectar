#include "generator-controller.h"
#include "data.h"

uint8_t generatorController_run(void) {
    SET_STATUS(GENERATOR_STATUS);
    return 1;
}

uint8_t generatorController_stop(void) {
    CLEAR_STATUS(GENERATOR_STATUS);
    return 1;
}

uint8_t pwmController_run(void) {
    SET_STATUS(PWM_STATUS);
    return 1;
}

uint8_t pwmController_stop(void) {
    CLEAR_STATUS(PWM_STATUS);
    return 1;
}

uint8_t mpptController_run(void) {
    SET_STATUS(MPPT_STATUS);
    return 1;
}

uint8_t mpptController_stop(void) {
    CLEAR_STATUS(MPPT_STATUS);
    return 1;
}
