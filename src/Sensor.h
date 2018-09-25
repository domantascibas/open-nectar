#ifndef SENSOR_H
#define SENSOR_H

static const uint16_t SAMPLE_NUM = 128; //for calibration SAMPLES << 3

struct Sensor {
  Sensor(const uint8_t addr);
  bool ready_to_sample;

  uint8_t ping();
  void calibrate();
  float sample(const uint16_t = SAMPLE_NUM);
  float get_reference();
  void set_reference(float ref);

private:
  Ticker t;
  const uint8_t address;
  float reference;
  char cmd[2];

  void start_sampling();
  void attach_ticker();
  void detach_ticker();
};

#endif
