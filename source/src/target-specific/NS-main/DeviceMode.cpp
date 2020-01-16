#include "DeviceMode.h"
#include "device_modes.h"
#include "Storage.h"
#include "error_controller.h"

// DeviceMode deviceOpMode;

static opModes currentMode;
static uint8_t testMode;
static uint8_t onboarding;
static uint8_t configured;
static uint8_t pairing;
static uint8_t reset;
static uint8_t boostOff;
static uint8_t inTestStand;
static uint8_t loading;

void deviceOpMode_init(void) {
  currentMode = WELCOME;
  testMode = 0;
  onboarding = 1;
  configured = 0;
  pairing = 1;
  reset = 0;
  boostOff = 0;
  inTestStand = 0;
  loading = 1;
}

uint8_t deviceOpMode_getCurrentMode(void) {
  return currentMode;
}

uint8_t DeviceMode::getCurrentMode() {
  // return currentMode;
  return 0;
}

void deviceOpMode_setConfigured(void) {
  currentMode = CONFIGURED;
  configured = 1;
}

void DeviceMode::setConfigured() {
  // currentMode = CONFIGURED;
  // configured = true;
}

void deviceOpMode_resetConfiguration(void) {
  currentMode = WELCOME;
  testMode = 0;
  configured = 0;
  onboarding = 1;
  // device_modes::reset();
  // Storage::clearConfig();
}

void DeviceMode::resetConfiguration() {
  // currentMode = WELCOME;
  // testMode = false;
  // configured = false;
  // onboarding = true;
}

void deviceOpMode_setTestMode(void) {
  testMode = 1;
  deviceOpMode_endOnboarding();
  currentMode = TEST_MODE;
}

void DeviceMode::setTestMode() {
  // testMode = true;
  // endOnboarding();
  // currentMode = TEST_MODE;
}

void deviceOpMode_endOnboarding(void) {
  if (!configured) {
    currentMode = NOT_CONFIGURED;
  } else {
    currentMode = CONFIGURED;
  }
  onboarding = false;
  if (!testMode) {
    // Storage::saveConfig();
  }
}

void DeviceMode::endOnboarding() {
  // if(!configured) currentMode = NOT_CONFIGURED;
  // else currentMode = CONFIGURED;
  // onboarding = false;
  // if(!testMode) Storage::saveConfig();
}

void deviceOpMode_setInTestStand(void) {
  inTestStand = 1;
	mainBoardError.clear_error(NO_BOILER_TEMP);
}

void DeviceMode::setInTestStand() {
  // inTestStand = true;
	// mainBoardError.clear_error(NO_BOILER_TEMP);
}

uint8_t deviceOpMode_isInTestMode(void) {
  return testMode;
}

bool DeviceMode::isInTestMode() {
  // return testMode;
  return 0;
}

uint8_t deviceOpMode_isOnboarding(void) {
  return onboarding;
}

bool DeviceMode::isOnboarding() {
  // return onboarding;
  return 0;
}

uint8_t deviceOpMode_isConfigured(void) {
  return configured;
}

bool DeviceMode::isConfigured() {
  // return configured;
  return 0;
}

uint8_t deviceOpMode_isPairing(void) {
  return pairing;
}

bool DeviceMode::isPairing() {
  // return pairing;
  return 0;
}

uint8_t deviceOpMode_isReset(void) {
  return reset;
}

bool DeviceMode::isReset() {
  // return reset;
  return 0;
}

uint8_t deviceOpMode_isBoostOff(void) {
  return boostOff;
}

bool DeviceMode::isBoostOff() {
  // return boostOff;
  return 0;
}

uint8_t deviceOpMode_isInTestStand(void) {
  return inTestStand;
}

bool DeviceMode::isInTestStand() {
  // return inTestStand;
  return 0;
}

uint8_t deviceOpMode_isLoading(void) {
  return loading;
}

bool DeviceMode::isLoading() {
  // return loading;
  return 0;
}

void deviceOpMode_endLoading(void) {
  loading = false;
}

void DeviceMode::endLoading() {
  // loading = false;
}

void deviceOpMode_setPairing(uint8_t input) {
  pairing = input;
}

void DeviceMode::setPairing(bool input) {
  // pairing = input;
}

void deviceOpMode_setReset(uint8_t input) {
  reset = input;
}

void DeviceMode::setReset(bool input) {
  // reset = input;
}

void deviceOpMode_setBoostOff(uint8_t input) {
  boostOff = input;
}

void DeviceMode::setBoostOff(bool input) {
  // boostOff = input;
}
