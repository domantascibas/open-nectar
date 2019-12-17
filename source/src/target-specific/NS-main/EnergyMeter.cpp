#include "consts.h"
#include "pins.h"
#include "EnergyMeter.h"
#include "DataService.h"

// EnergyMeter gridMeter(DataService::getBoilerPower);

void EnergyMeter::startMeter() {
  // if(meterSet) {
  //   ticker.attach(callback(this, &EnergyMeter::increaseMeter), ENERGY_METER_UPDATE_INTERVAL);
  // } else {
  //   printf("[INFO] energy meter not set\n");
  // }
}

void EnergyMeter::stopMeter(bool increase) {
  // ticker.detach();
  // if(increase) increaseMeter();
}

void EnergyMeter::increaseMeter() {
  // reading += getPower() * ENERGY_METER_UPDATE_INTERVAL / 3600 / 1000;
  // printf("ENERGY GRID %f\n", reading);
}

void EnergyMeter::setMeterReading(float value) {
  // meterSet = true;
  // reading = value;
}

float EnergyMeter::getMeterReading() {
  // return reading;
}

bool EnergyMeter::isMeterSet() {
  // return meterSet;
}
