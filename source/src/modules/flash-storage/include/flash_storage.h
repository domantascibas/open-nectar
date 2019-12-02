#ifndef _NS_FLASH_STORAGE_H
#define _NS_FLASH_STORAGE_H

void flash_storage_init(void);
bool flash_storage_load_data(void);
void flash_storage_save_data(void);
void flash_storage_save_meters(void);

#endif
