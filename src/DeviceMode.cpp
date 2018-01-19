#include "DeviceMode.h"
#include "device_modes.h"
#include "Storage.h"

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
  Storage::clearConfig();
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
  if(!testMode) Storage::saveConfig();
}

void DeviceMode::setInTestStand() {
  inTestStand = true;
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

bool DeviceMode::isInTestStand() {
  return inTestStand;
}

bool DeviceMode::isLoading() {
  return loading;
}

void DeviceMode::endLoading() {
  loading = false;
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
