#include "EnergyMeter.h"
#include "DataService.h"

EnergyMeter gridMeter(DataService::getBoilerPower);
//EnergyMeter sunMeter(DataService::getBoilerPower);

static const float INTERVAL = 5.0;

void EnergyMeter::startMeter() {
  if(meterSet) {
//    lastMomentPower = getPower();
    ticker.attach(callback(this, &EnergyMeter::increaseMeter), INTERVAL);
  } else {
    printf("energy meter not set\r\n");
  }
}

void EnergyMeter::stopMeter() {
  ticker.detach();
  increaseMeter();
}

void EnergyMeter::increaseMeter() {
//  float momentPower = ;
  reading += getPower() * INTERVAL / 3600 / 1000;
//  lastMomentPower = momentPower;
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
