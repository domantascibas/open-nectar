#ifndef OPERATIONAL_MODE_H
#define OPERATIONAL_MODE_H

#include <stdint.h>
#include "data.h"

enum operationalModes {
  WELCOME,
  NOT_CONFIGURED,
  CONFIGURED,
  TEST_MODE
};

struct OperationalMode {
  OperationalMode()
  : currentMode(WELCOME), inTestMode(false), inOnboarding(true), isConfigured(false) {};
    
  operationalModes currentMode;
  bool inTestMode;
  bool inOnboarding;
  bool isConfigured;
    
  uint8_t getCurrentMode();
  void endOnboarding();
  void setConfigured();
  void setTestMode();
  float *getTemperature();
  HeaterMode *getHeaterMode();
};

extern OperationalMode deviceOpMode;

#endif
