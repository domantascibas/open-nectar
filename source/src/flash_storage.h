#ifndef _NS_FLASH_STORAGE_H
#define _NS_FLASH_STORAGE_H

void flash_storage_init(void);
bool flash_storage_load_data(float *, float *, float *, float *);
void flash_storage_save_data(float, float);
void flash_storage_save_meters(float, float);

#endif
