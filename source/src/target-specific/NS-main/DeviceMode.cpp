#include "DeviceMode.h"
#include "device_modes.h"
#include "Storage.h"
#include "error_controller.h"

// DeviceMode deviceOpMode;

uint8_t DeviceMode::getCurrentMode() {
  // return currentMode;
  return 0;
}

void DeviceMode::setConfigured() {
  // currentMode = CONFIGURED;
  // configured = true;
}

void DeviceMode::resetConfiguration() {
  // currentMode = WELCOME;
  // testMode = false;
  // configured = false;
  // onboarding = true;
  // device_modes::reset();
  // Storage::clearConfig();
}

void DeviceMode::setTestMode() {
  // testMode = true;
  // endOnboarding();
  // currentMode = TEST_MODE;
}

void DeviceMode::endOnboarding() {
  // if(!configured) currentMode = NOT_CONFIGURED;
  // else currentMode = CONFIGURED;
  // onboarding = false;
  // if(!testMode) Storage::saveConfig();
}

void DeviceMode::setInTestStand() {
  // inTestStand = true;
	// mainBoardError.clear_error(NO_BOILER_TEMP);
}

bool DeviceMode::isInTestMode() {
  // return testMode;
  return 0;
}

bool DeviceMode::isOnboarding() {
  // return onboarding;
  return 0;
}

bool DeviceMode::isConfigured() {
  // return configured;
  return 0;
}

bool DeviceMode::isPairing() {
  // return pairing;
  return 0;
}

bool DeviceMode::isReset() {
  // return reset;
  return 0;
}

bool DeviceMode::isBoostOff() {
  // return boostOff;
  return 0;
}

bool DeviceMode::isInTestStand() {
  // return inTestStand;
  return 0;
}

bool DeviceMode::isLoading() {
  // return loading;
  return 0;
}

void DeviceMode::endLoading() {
  // loading = false;
}

void DeviceMode::setPairing(bool input) {
  // pairing = input;
}

void DeviceMode::setReset(bool input) {
  // reset = input;
}

void DeviceMode::setBoostOff(bool input) {
  // boostOff = input;
}
