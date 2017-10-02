#include "mbed.h"
#include "storage.h"
#include "eeprom.h"
#include "data.h"

//data written to and read from memory address 0x08002000
static const uint8_t DEVICE_CALIBRATED = 0xCA;
static const float V_REF = 3.00;

union {
  float f;
  uint16_t b[2];
} u;

enum data {
  CALIB = 0,
  V = 1,
  I = 3,
  SUN = 5,
  GRID = 7
};

//max number that can be saved from float - 65504 kWh. Worst case - 7yrs
//should change to uint32_t
uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555, 0x6555, 0x7555, 0x8555, 0x9555, 0x8888, 0x9888, 0xA888, 0xB888};
uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
uint16_t VarValue = 0;

namespace storage {
  bool locked = true;
    
  float read(uint8_t data) {
    EE_ReadVariable(VirtAddVarTab[data], &VarDataTab[data]);
    EE_ReadVariable(VirtAddVarTab[data+1], &VarDataTab[data+1]);
    u.b[1] = VarDataTab[data];
    u.b[0] = VarDataTab[data+1];
    return u.f;
  }

  uint8_t write(float voltage, float current) {
    if(locked) {
      return FLASH_ACCESS_ERROR;
    } else {
      u.f = voltage;
      EE_WriteVariable(VirtAddVarTab[V], u.b[1]);
      EE_WriteVariable(VirtAddVarTab[V+1], u.b[0]);
      
      u.f = current;
      EE_WriteVariable(VirtAddVarTab[I], u.b[1]);
      EE_WriteVariable(VirtAddVarTab[I+1], u.b[0]);
      
      EE_WriteVariable(VirtAddVarTab[CALIB], DEVICE_CALIBRATED);
      return NS_OK;
    }
  }
  
  uint8_t write_meters(float sun, float grid ){
    if(locked) {
      return FLASH_ACCESS_ERROR;
    } else {
      u.f = sun;
      EE_WriteVariable(VirtAddVarTab[SUN], u.b[1]);
      EE_WriteVariable(VirtAddVarTab[SUN+1], u.b[0]);
      
      u.f = grid;
      EE_WriteVariable(VirtAddVarTab[GRID], u.b[1]);
      EE_WriteVariable(VirtAddVarTab[GRID+1], u.b[0]);
      return NS_OK;
    }
  }
  
  uint8_t unlock() {
    if(FLASH->CR & (0x1 << 0x7)) {
      FLASH->KEYR = 0x45670123; //FLASH KEY 1
      FLASH->KEYR = 0xCDEF89AB; //FLASH KEY 2
      return NS_OK;
    } else {
      return FLASH_ACCESS_ERROR;
    }
  }

  uint8_t init() {
    uint8_t response;
    if(locked) {
      response = unlock();
      if(response == NS_OK) {
        locked = false;
      } else {
        return response;
      }
    }
    EE_Init();
    EE_ReadVariable(VirtAddVarTab[CALIB], &VarDataTab[CALIB]);
    if(VarDataTab[CALIB] == DEVICE_CALIBRATED) {
      data.calibrated = true;
      data.reference_voltage = read(V);
      data.reference_current = read(I);
    } else {
      data.calibrated = false;
      data.reference_voltage = 0.0;
      data.reference_current = 0.0;
    }
    data.sun_energy_meter_kwh = read(SUN);
    data.grid_energy_meter_kwh = read(GRID);
    return NS_OK;
  }

  uint8_t saveData(float voltage, float current) {
    return write(voltage, current);
  }
  
  uint8_t saveMeters(float sun, float grid) {
    return write_meters(sun, grid);
  }

  char testRead() {
    //	float v, i;
    //	int cal;
    EE_Init();

    //	EE_ReadVariable(VirtAddVarTab[0], &VarDataTab[0]);
    //	EE_ReadVariable(VirtAddVarTab[1], &VarDataTab[1]);
    EE_ReadVariable(VirtAddVarTab[CALIB], &VarDataTab[CALIB]);

    //	v = VarDataTab[0];
    //	i = VarDataTab[1];
    return VarDataTab[CALIB];
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
