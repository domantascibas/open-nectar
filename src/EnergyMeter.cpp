#include "consts.h"
#include "EnergyMeter.h"
#include "DataService.h"

EnergyMeter gridMeter(DataService::getBoilerPower);

void EnergyMeter::startMeter() {
  if(meterSet) {
    ticker.attach(callback(this, &EnergyMeter::increaseMeter), ENERGY_METER_UPDATE_INTERVAL);
  } else {
    printf("energy meter not set\r\n");
  }
}

void EnergyMeter::stopMeter(bool increase) {
  ticker.detach();
  if(increase) increaseMeter();
}

void EnergyMeter::increaseMeter() {
  reading += getPower() * ENERGY_METER_UPDATE_INTERVAL / 3600 / 1000;
  printf("[ENERGY] grid: %fkWh\r\n", reading);
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
