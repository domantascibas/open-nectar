#include "mbed.h"
#include "storage.h"
#include "eeprom.h"
#include "data.h"
//data written to and read from memory address 0x08002000

static const uint8_t DEVICE_CALIBRATED = 0xCA;
static const float V_REF = 3.00;

enum data {
    V = 0,
    I = 1,
    CALIB = 2
};

uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555, 0x6666, 0x7777};
uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};
uint16_t VarValue = 0;

namespace storage {
  bool locked = true;
    
  float read(uint8_t data) {
    EE_ReadVariable(VirtAddVarTab[data], &VarDataTab[data]);
    return (VarDataTab[data] * V_REF) / 2048;
  }

  uint8_t write(float voltage, float current) {
    if(locked) {
      return FLASH_ACCESS_ERROR;
    } else {
      EE_WriteVariable(VirtAddVarTab[V], (voltage / V_REF) * 2049 );
      EE_WriteVariable(VirtAddVarTab[I], (current / V_REF) * 2049 );
      EE_WriteVariable(VirtAddVarTab[CALIB], DEVICE_CALIBRATED);
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
    return NS_OK;
  }

  uint8_t saveData(float voltage, float current) {
    return write(voltage, current);
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
