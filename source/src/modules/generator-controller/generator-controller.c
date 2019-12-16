#include "generator-controller.h"
#include "data.h"
#include <stdint.h>
#include <stdio.h>

uint8_t generatorController_run(void) {
    SET_STATUS(GENERATOR_STATUS);
    // printf("[GENERATOR] STARTED\r\n");
    return 1;
}

uint8_t pwmController_run(void) {
    SET_STATUS(PWM_STATUS);
    return 1;
}
