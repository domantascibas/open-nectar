#include "storage.h"
#include "eeprom.h"
#include "ErrorHandler.h"

void StoredItem::load(float *item) {
  EE_ReadVariable(lo_address, &u.b[1]);
  EE_ReadVariable(hi_address, &u.b[0]);
  *item =  u.f;
}
  
void StoredItem::save(float item) {
  u.f = item;
  EE_WriteVariable(lo_address, u.b[1]);
  EE_WriteVariable(hi_address, u.b[0]);
}
  
void StoredItem::read(float *item) {
  *item = u.f;
}

namespace Storage {
  static const uint8_t DEVICE_CALIBRATED = 0xCA;
  static const uint16_t CALIB_ADDRESS = 0x4555;
  
  StoredItem ref_voltage(0x5555);
  StoredItem ref_current(0x6555);
  StoredItem grid_meter(0x7555);
  StoredItem sun_meter(0x8555);
  
  void init() {
    uint16_t cal;
    
    if(FLASH->CR & (0x1 << 0x7)) {
      FLASH->KEYR = 0x45670123; //FLASH KEY 1
      FLASH->KEYR = 0xCDEF89AB; //FLASH KEY 2
      nectarError.clear_error(FLASH_ACCESS_ERROR);
    }
    
    EE_Init();
    EE_ReadVariable(CALIB_ADDRESS, &cal);
    if(cal == DEVICE_CALIBRATED) nectarError.clear_error(CALIBRATION_ERROR);
  }
  
  void load_data(float *voltage, float *current, float *sun, float *grid) {
    ref_voltage.load(voltage);
    ref_current.load(current);
    sun_meter.load(sun);
    grid_meter.load(grid);
  }
  
  void save_data(float voltage, float current) {
    ref_voltage.save(voltage);
    ref_current.save(current);
    
    EE_WriteVariable(CALIB_ADDRESS, DEVICE_CALIBRATED);
  }
  
  void save_meters(float sun, float grid) {
    sun_meter.save(sun);
    grid_meter.save(grid);
  }
};

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
