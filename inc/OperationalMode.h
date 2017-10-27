#ifndef OPERATIONAL_MODE_H
#define OPERATIONAL_MODE_H

#include <stdint.h>
#include "data.h"
#include "menu_service.h"

enum operationalModes {
  WELCOME,
  NOT_CONFIGURED,
  CONFIGURED,
  TEST_MODE
};

struct OperationalMode {
  OperationalMode()
  : currentMode(WELCOME), inTestMode(false), inOnboarding(true), isConfigured(false), isPairing(true) {};
    
  operationalModes currentMode;
  bool inTestMode;
  bool inOnboarding;
  bool isConfigured;
  bool isPairing;
    
  uint8_t getCurrentMode();
  void endOnboarding();
  void setConfigured();
  void setTestMode();
  float getTemperature();
  float getTemperatureMax();
  nectar_contract::HeaterMode getHeaterMode();
  time_t timeHMtoTime(time_hm);
};

extern OperationalMode deviceOpMode;

#endif
