#include "mbed.h"
#include "MpptController.h"
#include "PwmController.h"
#include "data.h"

#define PWM_STEP 0.02

PwmController pwmGenerator(1.8, 0.1, 0.95);

MpptController::MpptController() {
  last_increase = true;
}

void MpptController::init() {
  pwmGenerator.init();
}

void MpptController::track() {
  static float old_power = 0.0;
  float moment_power;
  float dP;
  
  moment_power = data.moment_current * data.moment_voltage;
  data.moment_power = moment_power;
  dP = moment_power - old_power;

  pwmGenerator.start();
  if(dP != 0) {
    if(dP > 0) {
      if(last_increase) {
        pwmGenerator.increase_duty(PWM_STEP);
        last_increase = true;
      } else {
        pwmGenerator.decrease_duty(PWM_STEP);
        last_increase = false;
      }
    } else {
      if(last_increase) {
        pwmGenerator.decrease_duty(PWM_STEP);
        last_increase = false;
      } else {
        pwmGenerator.increase_duty(PWM_STEP);
        last_increase = true;
      }
    }
    old_power = moment_power;
  } else {
    if(last_increase) {
      pwmGenerator.increase_duty(PWM_STEP);
      last_increase = true;
    } else {
      pwmGenerator.decrease_duty(PWM_STEP);
      last_increase = false;
    }
  }
}

void MpptController::reset() {
  last_increase = true;
  pwmGenerator.stop();
  pwmGenerator.set_duty(0.1);
  data.moment_power = 0;
}

void MpptController::swipe(float min, float max, float step) {
  static bool reverse = false;
  
  pwmGenerator.start();
  if(!reverse) {
    if(pwmGenerator.get_duty() >= max) {
      reverse = true;
    } else {
      pwmGenerator.increase_duty(PWM_STEP);
      reverse = false;
    }
  } else {
    if(pwmGenerator.get_duty() <= min) {
      reverse = false;
    } else {
      pwmGenerator.decrease_duty(PWM_STEP);
      reverse = true;
    }
  }
}

float MpptController::get_duty() {
  return pwmGenerator.get_duty();
}

bool MpptController::is_generator_on() {
  return pwmGenerator.is_on();
}