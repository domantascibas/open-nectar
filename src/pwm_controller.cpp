#include "consts.h"
#include "pwm_controller.h"

float clamp(float);

static float duty = PWM_DUTY_MIN;

PwmOut pwmOut(PWM_PIN);

void pwm_controller_init(void) {
  duty = PWM_DUTY_MIN;
  pwmOut.period_us(1000/PWM_FREQUENCY);
  pwmOut.write(duty);

  printf("pwm controller setup\r\n");
}

void pwm_controller_increase_duty(float step) {
  duty = clamp(duty + step);
  pwmOut.write(duty);
}

void pwm_controller_decrease_duty(float step) {
  duty = clamp(duty - step);
  pwmOut.write(duty);
}

void pwm_controller_set_duty(float value) {
  duty = clamp(value);
  pwmOut.write(duty);
}

float pwm_controller_get_duty(void) {
  return duty;
}

float clamp(float input) {
  if(input >= PWM_DUTY_MAX) return PWM_DUTY_MAX;
  if(input < PWM_DUTY_MIN) return PWM_DUTY_MIN;
  return input;
}
