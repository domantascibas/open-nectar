#include "consts.h"
#include "Sensor.h"
#include "data.h"

#define REG_ADDR_RESULT         0x00
#define REG_ADDR_ALERT          0x01
#define REG_ADDR_CONFIG         0x02
#define REG_ADDR_LIMITL         0x03
#define REG_ADDR_LIMITH         0x04
#define REG_ADDR_HYST           0x05
#define REG_ADDR_CONVL          0x06
#define REG_ADDR_CONVH          0x07

I2C m_i2c(SDA, SCL);

Sensor::Sensor(const uint8_t addr) 
  : address(addr), ready_to_sample(false) {
    reference = 0.0;
    m_i2c.frequency(400000);
    attach_ticker();
}
  
void Sensor::attach_ticker() {
  t.attach(callback(this, &Sensor::start_sampling), 0.5);
}

void Sensor::detach_ticker() {
  t.detach();
}

uint8_t Sensor::ping() {
  uint8_t error;
  
  cmd[0] = REG_ADDR_CONFIG;
  cmd[1] = 0x20;

  error = m_i2c.write(address, cmd, 0);
  if(error == 0) {
    printf("[ok] ADC at 0x%X found\r\n", address);
    m_i2c.write(address, cmd, 2, false);
    return 1;
  } else {
    printf("[ERROR] ADC at 0x%X not found!\r\n", address);
    return 0;
  }
}

void Sensor::start_sampling() {
  ready_to_sample = true;
}

float Sensor::sample(const uint16_t samples) {
  float raw;
  float sum = 0;
  float avg = 0;
  uint16_t i = 0;
  
  ready_to_sample = false;
  
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
  return avg;
}

void Sensor::calibrate() {
  detach_ticker();
  reference = sample(SAMPLE_NUM << 3);
  attach_ticker();
}

float Sensor::get_reference() {
  return reference;
}

void Sensor::set_reference(float ref) {
  reference = ref;
}
