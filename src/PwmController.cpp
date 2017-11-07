#include "mbed.h"
#include "PwmController.h"

static const PinName PWM = PB_1;
static const PinName SD = PB_0;

PwmOut pwmOut(PWM);
DigitalOut shutdown(SD);

void PwmController::init() {
  duty = 0.1;
  printf("PWM Controller setup\r\n");
  pwmOut.period_us(1000/frequency);
  pwmOut.write(duty_min);
  stop();
}

void PwmController::start() {
  generator_on = true;
  shutdown = DRIVER_ON;
}

void PwmController::stop() {
  generator_on = false;
  shutdown = DRIVER_OFF;
}

void PwmController::increase_duty(float step) {
  duty = clamp(duty + step);
  pwmOut.write(duty);
}

void PwmController::decrease_duty(float step) {
  duty = clamp(duty - step);
  pwmOut.write(duty);
}

void PwmController::set_duty(float value) {
  duty = clamp(value);
  pwmOut.write(duty);
}

float PwmController::get_duty() {
//  printf("PWM DUTY: %f\r\n", duty);
  return duty;
}

float PwmController::clamp(float input) {
  if(input >= duty_max) { return duty_max; }
  if(input <= duty_min) { return duty_min; }
  return input;
}

bool PwmController::is_on() {
//  printf("GENERATOR ON: %d\r\n", generator_on);
  return generator_on;
}