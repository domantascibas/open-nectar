#include "DeviceMode.h"
#include "device_modes.h"

DeviceMode deviceOpMode;

uint8_t DeviceMode::getCurrentMode() {
  return currentMode;
}

void DeviceMode::setConfigured() {
  currentMode = CONFIGURED;
  configured = true;
}

void DeviceMode::resetConfiguration() {
  currentMode = WELCOME;
  testMode = false;
  configured = false;
  onboarding = true;
  device_modes::reset();
}

void DeviceMode::setTestMode() {
  testMode = true;
  endOnboarding();
  currentMode = TEST_MODE;
}

void DeviceMode::endOnboarding() {
  if(!configured) currentMode = NOT_CONFIGURED;
  else currentMode = CONFIGURED;
  onboarding = false;
}

bool DeviceMode::isInTestMode() {
  return testMode;
}

bool DeviceMode::isOnboarding() {
  return onboarding;
}

bool DeviceMode::isConfigured() {
  return configured;
}

bool DeviceMode::isPairing() {
  return pairing;
}

bool DeviceMode::isReset() {
  return reset;
}

bool DeviceMode::isBoostOff() {
  return boostOff;
}

void DeviceMode::setPairing(bool input) {
  pairing = input;
}

void DeviceMode::setReset(bool input) {
  reset = input;
}

void DeviceMode::setBoostOff(bool input) {
  boostOff = input;
}
