#ifndef POWER_GENERATOR_CONTROLLER_H
#define POWER_GENERATOR_CONTROLLER_H

#include <stdint.h>

uint8_t generatorController_run(void);
uint8_t generatorController_stop(void);
uint8_t pwmController_run(void);
uint8_t pwmController_stop(void);
uint8_t mpptController_run(void);
uint8_t mpptController_stop(void);

#endif
