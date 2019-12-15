#include "generator-controller.h"
#include "data.h"
#include <stdint.h>
#include <stdio.h>

uint8_t generatorController_run(void) {
    SET_STATUS(GENERATOR_STATUS);
    printf("GENERATOR STARTED\r\n");
    return 1;
}
