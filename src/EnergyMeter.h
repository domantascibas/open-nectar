#ifndef _NS_ENERGY_METER_H
#define _NS_ENERGY_METER_H

#include "mbed.h"

struct EnergyMeter {
  EnergyMeter(float (*getPower)()) : getPower(getPower), meterSet(false), reading(0.0) {};
  void startMeter();
  void stopMeter(bool);
  void setMeterReading(float);
  float getMeterReading();
  bool isMeterSet();

private:
  Ticker ticker;
  float reading;
  float lastMomentPower;
  bool meterSet;
  float (*getPower)();
  void increaseMeter();
};

extern EnergyMeter gridMeter;

#endif
