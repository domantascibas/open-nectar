#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "consts.h"
#include "settings.h"

using namespace utest::v1;

utest::v1::status_t test_setup(const size_t number_of_cases) {
  // Setup Greentea using a reasonable timeout in seconds
  GREENTEA_SETUP(40, "default_auto");
  return verbose_test_setup_handler(number_of_cases);
}

void check_dc_input_limits(void) {
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(REL_VOLTAGE_LIMIT, VOLTAGE_LIMIT, "wrong voltage limit");
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(REL_CURRENT_LIMIT, CURRENT_LIMIT, "wrong current limit");
}

void check_calibration_limits(void) {
  if(CALIBRATION_VOLTAGE_MAX <= CALIBRATION_VOLTAGE_MIN) TEST_ASSERT_MESSAGE(false, "voltage limit max value < min value");
  if(CALIBRATION_CURRENT_MAX <= CALIBRATION_CURRENT_MIN) TEST_ASSERT_MESSAGE(false, "current limit max value < min value");
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(REL_CALIBRATION_VOLTAGE_MAX, CALIBRATION_VOLTAGE_MAX, "wrong calibration voltage max limit");
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(REL_CALIBRATION_VOLTAGE_MIN, CALIBRATION_VOLTAGE_MIN, "wrong calibration voltage min limit");
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(REL_CALIBRATION_CURRENT_MAX, CALIBRATION_CURRENT_MAX, "wrong calibration current max limit");
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(REL_CALIBRATION_CURRENT_MIN, CALIBRATION_CURRENT_MIN, "wrong calibration current min limit");
}

void check_temperature_limits(void) {
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(REL_PROCESSOR_INTERNAL_TEMPERATURE_LIMIT, PROCESSOR_INTERNAL_TEMPERATURE_LIMIT, "wrong processor internal temperature limit");
}

void check_pwm_limits(void) {
  if(PWM_DUTY_MAX <= PWM_DUTY_MIN) TEST_ASSERT_MESSAGE(false, "pwm duty max < min");
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(REL_PWM_DUTY_MAX, PWM_DUTY_MAX, "wrong pwm duty max");
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(REL_PWM_DUTY_MIN, PWM_DUTY_MIN, "wrong pwm duty min");
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(REL_PWM_STEP, PWM_STEP, "wrong pwm duty step");
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(REL_PWM_STEP_FINE, PWM_STEP_FINE, "wrong pwm duty step fine");
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(REL_PWM_FREQUENCY, PWM_FREQUENCY, "wrong pwm frequency");
}

void check_other_limits(void) {
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(REL_CURRENT_THRESHOLD, CURRENT_THRESHOLD, "wrong current threshold");
}

// Test cases
Case cases[] = {
  Case("Settings: check DC input limits", check_dc_input_limits),
  Case("Settings: check calibration limits", check_calibration_limits),
  Case("Settings: check temperature limits", check_temperature_limits),
  Case("Settings: check pwm limits", check_pwm_limits),
  Case("Settings: check other limits", check_other_limits)
};

Specification specification(test_setup, cases);

// Entry point into the tests
int main() {
  return !Harness::run(specification);
}