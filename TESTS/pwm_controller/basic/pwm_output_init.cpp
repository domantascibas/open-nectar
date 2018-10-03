#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "pwm_output_tests.h"

using namespace utest::v1;

void pwm_output_init(void) {
  pwm_generator_init();
  
  if(pwm_generator_get_duty() != PWM_DUTY_MIN) {
    TEST_ASSERT_MESSAGE(false, "pwm generator init, duty != duty_min");
  }
  
  if(pwm_generator_is_on()) {
    TEST_ASSERT_MESSAGE(false, "pwm generator init, generator is on");
  }
  
  TEST_ASSERT(true);
}