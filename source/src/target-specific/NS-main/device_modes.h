#ifndef _NS_DEVICE_MODES_H
#define _NS_DEVICE_MODES_H

#include "RelayController.h"

void device_modes_update_mode_ISR(void);
void device_modes_setHeaterMode(uint8_t i);
void device_modes_setup(void);
void device_modes_reset(void);
void device_modes_loop(void);

#endif
