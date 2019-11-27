#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "consts.h"
#include "pwm_controller.h"

using namespace utest::v1;

utest::v1::status_t test_setup(const size_t number_of_cases) {
  // Setup Greentea using a reasonable timeout in seconds
  GREENTEA_SETUP(40, "default_auto");
  return verbose_test_setup_handler(number_of_cases);
}

void pwm_init(void) {
  pwm_controller_init();
  if(pwm_controller_get_duty() != PWM_DUTY_MIN) TEST_ASSERT_MESSAGE(false, "pwm generator init, duty != duty_min");
  TEST_ASSERT(true);
}

template<int8_t val>
void pwm_set_duty(void) {
  float duty_val = (float)val / 100;
  pwm_controller_set_duty(duty_val);

  if((duty_val < PWM_DUTY_MAX) & (duty_val > PWM_DUTY_MIN)) {
    if(pwm_controller_get_duty() != duty_val) TEST_ASSERT_MESSAGE(false, "pwm duty set wrong");
  } else {
    if(duty_val >= PWM_DUTY_MAX) {
      if(pwm_controller_get_duty() != PWM_DUTY_MAX) TEST_ASSERT_MESSAGE(false, "pwm duty not clamped to PWM_DUTY_MAX");
    }
     
    if(duty_val <= PWM_DUTY_MIN) {
      if(pwm_controller_get_duty() != PWM_DUTY_MIN) TEST_ASSERT_MESSAGE(false, "pwm duty not clamped to PWM_DUTY_MIN");
    }
  }
  
  TEST_ASSERT(true);
}

template<int8_t val>
void pwm_reset_duty(void) {
  float duty_val = (float)val / 100;
  pwm_controller_init();
  pwm_controller_set_duty(duty_val);
  if(pwm_controller_get_duty() != duty_val) TEST_ASSERT_MESSAGE(false, "pwm duty set wrong");
  pwm_controller_init();
  if(pwm_controller_get_duty() != PWM_DUTY_MIN) TEST_ASSERT_MESSAGE(false, "pwm duty not reset to min");
  TEST_ASSERT(true);
}

template<int8_t val>
void pwm_increase_duty(void) {
  float duty_val = (float)val / 100;
  pwm_controller_set_duty(PWM_DUTY_MIN);
  pwm_controller_increase_duty(duty_val);
  if((PWM_DUTY_MIN + duty_val) >= PWM_DUTY_MAX) {
    if(pwm_controller_get_duty() != PWM_DUTY_MAX) TEST_ASSERT_MESSAGE(false, "pwm duty max not clamped");
  } else {
    if(pwm_controller_get_duty() != (PWM_DUTY_MIN + duty_val)) TEST_ASSERT_MESSAGE(false, "pwm duty increase error");
  }
  TEST_ASSERT(true);
}

template<int8_t val>
void pwm_decrease_duty(void) {
  float duty_val = (float)val / 100;
  pwm_controller_set_duty(PWM_DUTY_MAX);
  pwm_controller_decrease_duty(duty_val);
  if((PWM_DUTY_MAX - duty_val) <= PWM_DUTY_MIN) {
    if(pwm_controller_get_duty() != PWM_DUTY_MIN) TEST_ASSERT_MESSAGE(false, "pwm duty min not clamped");
  } else {
    if(pwm_controller_get_duty() != (PWM_DUTY_MAX - duty_val)) TEST_ASSERT_MESSAGE(false, "pwm duty decrease error");
  }
  TEST_ASSERT(true);
}

// Test cases
Case cases[] = {
  Case("Pwm: Init", pwm_init),
  Case("Pwm: Set Duty within limits 0.25", pwm_set_duty<25>),
  Case("Pwm: Set Duty within limits 0.50", pwm_set_duty<50>),
  Case("Pwm: Set Duty within limits 0.75", pwm_set_duty<75>),
  // Case("Pwm: Set Duty Max", pwm_set_duty<PWM_DUTY_MAX>),
  // Case("Pwm: Set Duty Min", pwm_set_duty<PWM_DUTY_MIN>),
  Case("Pwm: Set Duty above Max", pwm_set_duty<110>),
  Case("Pwm: Set Duty below Min", pwm_set_duty<-10>),
  Case("Pwm: Reset Duty", pwm_reset_duty<50>),
  Case("Pwm: Increase Duty", pwm_increase_duty<50>),
  Case("Pwm: Increase Duty above Max", pwm_increase_duty<100>),
  Case("Pwm: Decrease Duty", pwm_decrease_duty<50>),
  Case("Pwm: Decrease Duty below Min", pwm_decrease_duty<100>)
};

Specification specification(test_setup, cases);

// Entry point into the tests
int main() {
  return !Harness::run(specification);
}