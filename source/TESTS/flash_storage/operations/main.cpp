#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "consts.h"
#include "flash_storage.h"
#include "ErrorHandler.h"

using namespace utest::v1;

utest::v1::status_t test_setup(const size_t number_of_cases) {
  // Setup Greentea using a reasonable timeout in seconds
  GREENTEA_SETUP(40, "default_auto");
  return verbose_test_setup_handler(number_of_cases);
}

static const float VAL1 = 0.1234;
static const float VAL2 = 0.5678;
static const float VAL3 = 1.2345;
static const float VAL4 = 5.6789;

void init(void) {
  nectarError.set_error(FLASH_ACCESS_ERROR);
  flash_storage_init();
  TEST_ASSERT_MESSAGE(!nectarError.has_error(FLASH_ACCESS_ERROR), "flash access error, flash not initialised");
}

void save_data(void) {
  flash_storage_save_data(VAL1, VAL2);
  TEST_ASSERT_MESSAGE(!nectarError.has_error(FLASH_ACCESS_ERROR), "flash access error");
}

void save_meters(void) {
  flash_storage_save_meters(VAL3, VAL4);
  TEST_ASSERT_MESSAGE(!nectarError.has_error(FLASH_ACCESS_ERROR), "flash access error");
}

void load_data(void) {
  float val1;
  float val2;
  float val3;
  float val4;

  bool calibrated = flash_storage_load_data(&val1, &val2, &val3, &val4);
  TEST_ASSERT_MESSAGE(calibrated, "flash storage NOT calibrated");
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(VAL1, val1, "val1");
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(VAL2, val2, "val2");
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(VAL3, val3, "val3");
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(VAL4, val4, "val4");
}

// Test cases
Case cases[] = {
  Case("Flash Storage: Init", init),
  Case("Flash Storage: Save Data", save_data),
  Case("Flash Storage: Save Meters", save_meters),
  Case("Flash Storage: Load Data", load_data)
};

Specification specification(test_setup, cases);

// Entry point into the tests
int main() {
  return !Harness::run(specification);
}