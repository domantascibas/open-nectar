#ifndef _NS_FLASH_STORAGE_H
#define _NS_FLASH_STORAGE_H

#include <stdint.h>

static const uint8_t DEVICE_CALIBRATED = 0xCA;

void flash_storage_init(void);
uint8_t flash_storage_load_data(void);
void flash_storage_save_data(void);
void flash_storage_save_meters(void);

#endif
