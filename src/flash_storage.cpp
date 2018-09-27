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
  bool emptyStorage = true;
  
  if(FLASH->CR & (0x1 << 0x7)) {
    FLASH->KEYR = 0x45670123; //FLASH KEY 1
    FLASH->KEYR = 0xCDEF89AB; //FLASH KEY 2
    nectarError.clear_error(FLASH_ACCESS_ERROR);
  }
  
  EE_Init();
  EE_SettingsDatastruct rDataStruct;
  emptyStorage = EE_ReadDatastruct(&rDataStruct);
  
  if(emptyStorage) {
    printf("[STORAGE] empty\r\n");
    sDataStruct.device_calibrated = 0xFA;
  } else {
    printf("[STORAGE] not empty\r\n");
    printf("[STORAGE] %d %d %f %f\r\n", rDataStruct.device_calibrated, rDataStruct.extra_property, rDataStruct.ref_voltage, rDataStruct.ref_current);
    sDataStruct = rDataStruct;
    printf("[STORAGE] %d %d %f %f\r\n", sDataStruct.device_calibrated, sDataStruct.extra_property, sDataStruct.ref_voltage, sDataStruct.ref_current);
  }
  
  if(sDataStruct.device_calibrated == DEVICE_CALIBRATED) {
    nectarError.clear_error(CALIBRATION_ERROR);
    // if(((sDataStruct.ref_voltage < 0.002) && (sDataStruct.ref_voltage > 0.001)) && ((sDataStruct.ref_current < 2.2) && (sDataStruct.ref_current > 1.8))) {
    //   nectarError.clear_error(CALIBRATION_ERROR);
    // } else {
    //   nectarError.set_error(CALIBRATION_ERROR);
    // }
  }
}

void flash_storage_load_data(float *voltage, float *current, float *sun, float *grid) {
  bool emptyStorage = true;
  
  EE_SettingsDatastruct rDataStruct;
  emptyStorage = EE_ReadDatastruct(&rDataStruct);
  
  if(emptyStorage) {
    sDataStruct.device_calibrated = 0xFA;
  } else {
    sDataStruct = rDataStruct;			
    *voltage = sDataStruct.ref_voltage;
    *current = sDataStruct.ref_current;

    *sun = sDataStruct.sun_meter;
    *grid = sDataStruct.grid_meter;
  }
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
