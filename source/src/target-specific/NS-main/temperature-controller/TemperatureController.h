#ifndef _NS_TEMPERATURE_CONTROLLER_H
#define _NS_TEMPERATURE_CONTROLLER_H

void temperatureController_init(void);
uint8_t temperatureController_getBoilerTemp(void);
void temperatureController_update(void);

#endif
