#include "EnergyMeter.h"

void EnergyMeter::increaseMeter(float value) {
  reading += value;
}

void EnergyMeter::setMeterReading(float value) {
  meterSet = true;
  reading = value;
}

float EnergyMeter::getMeterReading() {
  return reading;
}

bool EnergyMeter::isMeterSet() {
  return meterSet;
}
