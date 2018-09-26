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

// void PwmController::init() {
//   duty = 0.1;
//   printf("PWM Controller setup\r\n");
//   pwmOut.period_us(1000/frequency);
//   pwmOut.write(duty_min);
//   stop();
// }

// void PwmController::start() {
//   generator_on = true;
//   shutdown = DRIVER_ON;
// }

// void PwmController::stop() {
//   generator_on = false;
//   shutdown = DRIVER_OFF;
// }

// void PwmController::increase_duty(float step) {
//   duty = clamp(duty + step);
//   pwmOut.write(duty);
// }

// void PwmController::decrease_duty(float step) {
//   duty = clamp(duty - step);
//   pwmOut.write(duty);
// }

// void PwmController::set_duty(float value) {
//   duty = clamp(value);
//   pwmOut.write(duty);
// }

// float PwmController::get_duty() {
// //  printf("PWM DUTY: %f\r\n", duty);
//   return duty;
// }

// float PwmController::clamp(float input) {
//   if(input >= duty_max) { return duty_max; }
//   if(input <= duty_min) { return duty_min; }
//   return input;
// }

// bool PwmController::is_on() {
// //  printf("GENERATOR ON: %d\r\n", generator_on);
//   return generator_on;
// }