#include "storage.h"
#include "eeprom.h"
#include "utils.h"

enum data {
    V = 0,
    I = 1
};

//Global vars
bool    locked = true;
bool    calibrationDone = false;

uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555, 0x6666, 0x7777};
uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};
uint16_t VarValue = 0;

char storage::testRead() {
//	float v, i;
//	int cal;
    EE_Init();
    
//	EE_ReadVariable(VirtAddVarTab[0], &VarDataTab[0]);
//	EE_ReadVariable(VirtAddVarTab[1], &VarDataTab[1]);
    EE_ReadVariable(VirtAddVarTab[2], &VarDataTab[2]);
    
//	v = VarDataTab[0];
//	i = VarDataTab[1];
    return VarDataTab[2];
}

int storage::init() {
    int response;
    if(locked) {
        response = unlock();
        if(response == FLASH_ACCESS_OK) {
            locked = false;
        } else {
            return response;
        }
    }
    EE_Init();
    calibrationDone = isCalibrated();
    if(calibrationDone) {
        return CALIBRATION_OK;
    } else {
        return CALIBRATION_ERROR;
    }
}

bool storage::getLock() {
    return locked;
}

bool storage::isCalibrated() {
    EE_ReadVariable(VirtAddVarTab[2], &VarDataTab[2]);
    if(VarDataTab[2] == DEVICE_CALIBRATED) {
        return true;
    } else {
        return false;
    }
}

float storage::getVoltage() {
    return read(V);
}

float storage::getCurrent() {
    return read(I);
}

int storage::saveData(float voltage, float current) {
    return write(voltage, current);
}

int storage::unlock() {
    if(FLASH->CR & (0x1 << 0x7)) {
        FLASH->KEYR = 0x45670123; //FLASH KEY 1
        FLASH->KEYR = 0xCDEF89AB; //FLASH KEY 2
        return FLASH_ACCESS_OK;
    } else {
        return FLASH_ACCESS_ERROR;
    }
}

int storage::write(float voltage, float current) {
    if(locked) {
        calibrationDone = false;
        return FLASH_WRITE_ERROR;
    } else {
        EE_WriteVariable(VirtAddVarTab[0], (voltage / V_REF) * 2049 );
        EE_WriteVariable(VirtAddVarTab[1], (current / V_REF) * 2049 );
        EE_WriteVariable(VirtAddVarTab[2], DEVICE_CALIBRATED);
        calibrationDone = true;
        return FLASH_WRITE_OK;
    }
}

float storage::read(int data) {
    if(locked) {
        return FLASH_ACCESS_ERROR;
    } else {
        if(calibrationDone) {
            EE_ReadVariable(VirtAddVarTab[data], &VarDataTab[data]);
            return (VarDataTab[data] * V_REF) / 2048;
        } else {
            return 0.00;
        }
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
