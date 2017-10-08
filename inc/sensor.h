#ifndef SENSOR_H
#define SENSOR_H

static const uint16_t SAMPLE_NUM = 128; //for calibration SAMPLES << 3

struct Sensor {
  Sensor(const uint8_t addr, float *ref);
  
  float *reference;
  bool ready_to_sample;
  
  uint8_t ping();
  void attach_ticker();
  void detach_ticker();
  float sample(const uint16_t = SAMPLE_NUM);
  
  private:
    const uint8_t address;
    char cmd[2];
    
    Ticker t;
    void start_sampling();
};

#endif
