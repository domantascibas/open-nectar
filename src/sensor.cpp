#include "mbed.h"
#include "sensor.h"

#define REG_ADDR_RESULT         0x00
#define REG_ADDR_ALERT          0x01
#define REG_ADDR_CONFIG         0x02
#define REG_ADDR_LIMITL         0x03
#define REG_ADDR_LIMITH         0x04
#define REG_ADDR_HYST           0x05
#define REG_ADDR_CONVL          0x06
#define REG_ADDR_CONVH          0x07

void Sensor::ping() {
  uint8_t error;
  
  cmd[0] = REG_ADDR_CONFIG;
  cmd[1] = 0x20;

  error = m_i2c.write(address, cmd, 0);
  if(error == 0) {
    printf("[ok] ADC at 0x%X found\r\n", address);
    m_i2c.write(address, cmd, 2, false);
//    return NS_OK;
  } else {
    printf("[ERROR] ADC at 0x%X not found!\r\n", address);
//    return NS_ERROR;
  }
}

void Sensor::sample(uint16_t samples) {
  float raw;
  float sum = 0;
  float avg = 0;
  uint16_t i = 0;

  for(i=0; i<samples; i++) {
    cmd[0] = REG_ADDR_RESULT;
    m_i2c.write(address, cmd, 1);
    m_i2c.read(address, cmd, 2);
    raw = ((cmd[0] & 0x0F) << 8) | cmd[1];
    sum += (raw * V_REF) / 2048; //raw * VREF * 2 / 4096
  }
  
  avg = sum / samples;
  if(avg < 0) {
    avg = 0;
  }
  //printf("ADC 0x%X %.2f\r\n", address, avg);
  value = avg;
}
