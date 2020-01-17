#include "DeviceMode.h"
#include "device_modes.h"
#include "Storage.h"

extern "C" {
#include "error_handler.h"
}

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

void deviceOpMode_setConfigured(void) {
  currentMode = CONFIGURED;
  configured = 1;
}

void deviceOpMode_resetConfiguration(void) {
  currentMode = WELCOME;
  testMode = 0;
  configured = 0;
  onboarding = 1;
  device_modes_reset();
  storage_clearConfig();
}

void deviceOpMode_setTestMode(void) {
  testMode = 1;
  deviceOpMode_endOnboarding();
  currentMode = TEST_MODE;
}

void deviceOpMode_endOnboarding(void) {
  if (!configured) {
    currentMode = NOT_CONFIGURED;
  } else {
    currentMode = CONFIGURED;
  }
  onboarding = false;
  if (!testMode) {
    storage_saveConfig();
  }
}

void deviceOpMode_setInTestStand(void) {
  inTestStand = 1;
  error_clear(NS_NO_BOILER_TEMP);
	// mainBoardError.clear_error(NO_BOILER_TEMP);
}

uint8_t deviceOpMode_isInTestMode(void) {
  return testMode;
}

uint8_t deviceOpMode_isOnboarding(void) {
  return onboarding;
}

uint8_t deviceOpMode_isConfigured(void) {
  return configured;
}

uint8_t deviceOpMode_isPairing(void) {
  return pairing;
}

uint8_t deviceOpMode_isReset(void) {
  return reset;
}

uint8_t deviceOpMode_isBoostOff(void) {
  return boostOff;
}

uint8_t deviceOpMode_isInTestStand(void) {
  return inTestStand;
}

uint8_t deviceOpMode_isLoading(void) {
  return loading;
}

void deviceOpMode_endLoading(void) {
  loading = false;
}

void deviceOpMode_setPairing(uint8_t input) {
  pairing = input;
}

void deviceOpMode_setReset(uint8_t input) {
  reset = input;
}

void deviceOpMode_setBoostOff(uint8_t input) {
  boostOff = input;
}
