#ifndef ENERGY_METER_H
#define ENERGY_METER_H

struct EnergyMeter {
  EnergyMeter();
  
  void increase(float);
  void load_reading(float);
  float get_reading();

private:
  float reading;
};

#endif
