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
  DeviceMode() : currentMode(WELCOME), testMode(false), onboarding(true), configured(false), pairing(true), reset(false), boostOff(false) {};
  uint8_t getCurrentMode();
  void endOnboarding();
  void setTestMode();
  void setConfigured();
  void resetConfiguration();
    
  bool isInTestMode();
  bool isOnboarding();
  bool isConfigured();
  bool isPairing();
  bool isReset();
  bool isBoostOff();
    
  void setPairing(bool);
  void setReset(bool);
  void setBoostOff(bool);

private:
  opModes currentMode;
  bool testMode;
  bool onboarding;
  bool configured;
  bool pairing;
  bool reset;
  bool boostOff;
};

extern DeviceMode deviceOpMode;

#endif
