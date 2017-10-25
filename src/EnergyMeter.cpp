#include "EnergyMeter.h"

EnergyMeter::EnergyMeter() {
  reading = 0.0;
}

void EnergyMeter::increase(float val) {
  reading += val;
}

void EnergyMeter::load_reading(float val) {
  reading = val;
}

float EnergyMeter::get_reading() {
  return reading;
}