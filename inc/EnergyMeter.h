#ifndef ENERGY_METER_H
#define ENERGY_METER_H

struct EnergyMeter {
  EnergyMeter() : meterSet(false), reading(0.0) {};
  void increaseMeter(float);
  void setMeterReading(float);
  float getMeterReading();
  bool isMeterSet();

private:
  float reading;
  bool meterSet;
};

#endif
