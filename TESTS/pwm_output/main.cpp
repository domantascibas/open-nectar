#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "pwm_output_tests.h"

using namespace utest::v1;

utest::v1::status_t test_setup(const size_t number_of_cases) {
  // Setup Greentea using a reasonable timeout in seconds
  GREENTEA_SETUP(40, "default_auto");
  return verbose_test_setup_handler(number_of_cases);
}

// Test cases
Case cases[] = {
  Case("Pwm-Output-Init", pwm_output_init)
};

Specification specification(test_setup, cases);

// Entry point into the tests
int main() {
  return !Harness::run(specification);
}