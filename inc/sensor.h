#ifndef SENSOR_H
#define SENSOR_H

static const PinName SDA = PB_14;
static const PinName SCL = PB_13;
static const float V_REF = 3.00;

static const uint8_t SAMPLES = 128; //for calibration SAMPLES << 3

struct Sensor {
  Sensor(const uint8_t addr)
    : address(addr), m_i2c(SDA, SCL) {
//      m_i2c.frequency = 400000;
    };
      
  float value;
  void ping();
  void sample(uint16_t samples = SAMPLES);    
  
  private:
    const uint8_t address;
    char cmd[2];
  
    I2C m_i2c;
};

#endif
