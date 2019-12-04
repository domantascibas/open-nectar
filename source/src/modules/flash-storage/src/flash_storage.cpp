// #include "consts.h"
// #include "data.h"
#include "eeprom.h"
#include "error_controller.h"
#include "flash_storage.h"

extern "C" {
  #include "data.h"
}

// static uint8_t calibrated = EMPTY_VALUE;
// static uint8_t extra_property = EMPTY_VALUE;
// static uint8_t extra_property2 = EMPTY_VALUE;
// static uint8_t extra_property3 = EMPTY_VALUE;

// EE_SettingsDatastruct sDataStruct = {
//   calibrated,
//   extra_property,
//   extra_property2,
//   extra_property3,
//   storage_data.ref_voltage,
//   storage_data.ref_current,
//   storage_data.sun_meter,
//   storage_data.grid_meter
// };

void flash_storage_init(void) {
  if(FLASH->CR & (0x1 << 0x7)) {
    FLASH->KEYR = 0x45670123; //FLASH KEY 1
    FLASH->KEYR = 0xCDEF89AB; //FLASH KEY 2
  }

  uint16_t result = EE_Init();
  if(result == HAL_OK) nectarError.clear_error(FLASH_ACCESS_ERROR);
}

bool flash_storage_load_data(void) {
// bool flash_storage_load_data(float *voltage, float *current, float *sun, float *grid) {
  bool load_error = true;

  EE_SettingsDatastruct rDataStruct;
  load_error = EE_ReadDatastruct(&rDataStruct);

  if(load_error) {
    printf("[STORAGE] empty\r\n");
    // sDataStruct.device_calibrated = EMPTY_VALUE;
    CLEAR_STATUS(CALIBRATION_STATUS);
    return 0;
  } else {
    printf("[STORAGE] not empty\r\n");
    // sDataStruct = rDataStruct;
    // *voltage = sDataStruct.ref_voltage;
    // *current = sDataStruct.ref_current;
    PowerData_write(R_VOLTAGE, &rDataStruct.ref_voltage);
    PowerData_write(R_CURRENT, &rDataStruct.ref_current);

    // *sun = sDataStruct.sun_meter;
    // *grid = sDataStruct.grid_meter;
    PowerData_write(SUN_METER, &rDataStruct.sun_meter);
    PowerData_write(GRID_METER, &rDataStruct.grid_meter);
    // printf("[STORAGE] %d %d %f %f\r\n", sDataStruct.device_calibrated, sDataStruct.extra_property, sDataStruct.ref_voltage, sDataStruct.ref_current);
    PowerData_info();
  }

  if (rDataStruct.device_calibrated == DEVICE_CALIBRATED) {
    SET_STATUS(CALIBRATION_STATUS);
  } else {
    CLEAR_STATUS(CALIBRATION_STATUS);
  }

  return 1;
}

// void flash_storage_save_data(float voltage, float current) {
void flash_storage_save_data(void) {
  EE_SettingsDatastruct rDataStruct;
  bool flash_write_error = true;

  // rDataStruct.ref_voltage = voltage;
  // rDataStruct.ref_current = current;
  // rDataStruct.device_calibrated = DEVICE_CALIBRATED;
  PowerData_read(R_VOLTAGE, &rDataStruct.ref_voltage);
  PowerData_read(R_CURRENT, &rDataStruct.ref_current);
  PowerData_read(SUN_METER, &rDataStruct.sun_meter);
  PowerData_read(GRID_METER, &rDataStruct.grid_meter);
  rDataStruct.device_calibrated = DEVICE_CALIBRATED;
  flash_write_error = EE_WriteDatastruct(&rDataStruct);

  if(flash_write_error) {
    nectarError.set_error(FLASH_ACCESS_ERROR);
  }
}

void flash_storage_save_meters(void) {
  if (GET_STATUS(CALIBRATION_STATUS)) {
    EE_SettingsDatastruct rDataStruct;
    bool flash_write_error = true;

    // sDataStruct.sun_meter = sun;
    // sDataStruct.grid_meter = grid;
    PowerData_read(R_VOLTAGE, &rDataStruct.ref_voltage);
    PowerData_read(R_CURRENT, &rDataStruct.ref_current);
    PowerData_read(SUN_METER, &rDataStruct.sun_meter);
    PowerData_read(GRID_METER, &rDataStruct.grid_meter);
    flash_write_error = EE_WriteDatastruct(&rDataStruct);

    if(flash_write_error) {
      nectarError.set_error(FLASH_ACCESS_ERROR);
    }
  } else {
    printf("[FS] Device not calibrated. Meters and ref not saved");
  }
}
