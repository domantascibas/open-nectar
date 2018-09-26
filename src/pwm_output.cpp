#include "consts.h"
#include "pwm_output.h"

float clamp(float);

static float duty = PWM_DUTY_MIN;
static bool generator_on = false;

PwmOut pwmOut(PWM_PIN);
DigitalOut shutdown(SD_PIN);

void pwm_output_init(void) {
  duty = PWM_DUTY_MIN;
  pwmOut.period_us(1000/PWM_FREQUENCY);
  pwmOut.write(duty);
  pwm_output_stop();

  printf("pwm controller setup\r\n");
}

void pwm_output_start(void) {
  generator_on = true;
  shutdown = DRIVER_ON;
}

void pwm_output_stop(void) {
  generator_on = false;
  shutdown = DRIVER_OFF;
}

void pwm_output_increase_duty(void) {
  duty = clamp(duty + PWM_STEP);
  pwmOut.write(duty);
}

void pwm_output_decrease_duty(void) {
  duty = clamp(duty - PWM_STEP);
  pwmOut.write(duty);
}

void pwm_output_increase_duty_fine(void) {
  duty = clamp(duty + PWM_STEP_FINE);
  pwmOut.write(duty);
}

void pwm_output_decrease_duty_fine(void) {
  duty = clamp(duty - PWM_STEP_FINE);
  pwmOut.write(duty);
}

void pwm_output_set_duty(float value) {
  duty = clamp(value);
  pwmOut.write(duty);
}

float pwm_output_get_duty(void) {
  return duty;
}

bool pwm_output_is_on(void) {
  return generator_on;
}

float clamp(float input) {
  if(input >= PWM_DUTY_MAX) return PWM_DUTY_MAX;
  if(input < PWM_DUTY_MIN) return PWM_DUTY_MIN;
  return input;
}