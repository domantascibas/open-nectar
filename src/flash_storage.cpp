#include "consts.h"
#include "data.h"
#include "flash_storage.h"
#include "eeprom.h"
#include "ErrorHandler.h"

static uint8_t calibrated = 0xFA;
static uint8_t extra_property = 0xFA;
static uint8_t extra_property2 = 0xFA;
static uint8_t extra_property3 = 0xFA;

EE_SettingsDatastruct sDataStruct = {
  calibrated,
  extra_property,
  extra_property2,
  extra_property3,
  storage_data.ref_voltage,
  storage_data.ref_current,
  storage_data.sun_meter,
  storage_data.grid_meter
};

void flash_storage_init(void) {
  // bool emptyStorage = true;
  
  if(FLASH->CR & (0x1 << 0x7)) {
    FLASH->KEYR = 0x45670123; //FLASH KEY 1
    FLASH->KEYR = 0xCDEF89AB; //FLASH KEY 2
    nectarError.clear_error(FLASH_ACCESS_ERROR);
  }
  
  EE_Init();
  // EE_SettingsDatastruct rDataStruct;
  // emptyStorage = EE_ReadDatastruct(&rDataStruct);
  
  // if(emptyStorage) {
  //   sDataStruct.device_calibrated = EMPTY_VALUE;
  // } else {
  //   sDataStruct = rDataStruct;
  //   data.calibrated = sDataStruct.device_calibrated;
  // }
}

bool flash_storage_load_data(float *voltage, float *current, float *sun, float *grid) {
  bool load_error = true;
  
  EE_SettingsDatastruct rDataStruct;
  load_error = EE_ReadDatastruct(&rDataStruct);
  
  if(load_error) {
    printf("[STORAGE] empty\r\n");
    sDataStruct.device_calibrated = EMPTY_VALUE;
  } else {
    printf("[STORAGE] not empty\r\n");
    sDataStruct = rDataStruct;			
    *voltage = sDataStruct.ref_voltage;
    *current = sDataStruct.ref_current;

    *sun = sDataStruct.sun_meter;
    *grid = sDataStruct.grid_meter;
    printf("[STORAGE] %d %d %f %f\r\n", sDataStruct.device_calibrated, sDataStruct.extra_property, sDataStruct.ref_voltage, sDataStruct.ref_current);
  }

  return (sDataStruct.device_calibrated == DEVICE_CALIBRATED);
}

void flash_storage_save_data(float voltage, float current) {
  bool flash_write_error = true;
  sDataStruct.ref_voltage = voltage;
  sDataStruct.ref_current = current;
  sDataStruct.device_calibrated = DEVICE_CALIBRATED;

  flash_write_error = EE_WriteDatastruct(&sDataStruct);
  if(flash_write_error) {
    nectarError.set_error(FLASH_ACCESS_ERROR);
  }
}

void flash_storage_save_meters(float sun, float grid) {
  bool flash_write_error = true;
  sDataStruct.sun_meter = sun;
  sDataStruct.grid_meter = grid;

  flash_write_error = EE_WriteDatastruct(&sDataStruct);
  if(flash_write_error) {
    nectarError.set_error(FLASH_ACCESS_ERROR);
  }
}
