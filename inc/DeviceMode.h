#ifndef DEVICE_MODE_H
#define DEVICE_MODE_H

#include <stdint.h>

enum opModes {
  WELCOME,
  NOT_CONFIGURED,
  CONFIGURED,
  TEST_MODE
};

struct DeviceMode {
  DeviceMode() : currentMode(WELCOME), testMode(false), onboarding(true), configured(false), pairing(true) {};
  uint8_t getCurrentMode();
  void endOnboarding();
  void setTestMode();
  void setConfigured();
  bool isInTestMode();
  bool isOnboarding();
  bool isConfigured();
  bool isPairing();

private:
  opModes currentMode;
  bool testMode;
  bool onboarding;
  bool configured;
  bool pairing;
};

extern DeviceMode deviceOpMode;

#endif
