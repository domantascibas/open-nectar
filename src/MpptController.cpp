#include "mbed.h"
#include "MpptController.h"
#include "PwmController.h"
#include "ErrorHandler.h"
#include "data.h"

static const float PWM_STEP = 0.02;
static const float POWER_THRESHOLD = 50.0;
static const PinName DEVICE_TEMP_PROBE = PC_7;
static const float DEVICE_TEMPERATURE_LIMIT_MAX = 85.0;

PwmController pwmGenerator(1.8, 0.1, 0.95);
TemperatureSensor deviceTemp(DEVICE_TEMP_PROBE, 10);

MpptController::MpptController() {
  last_increase = true;
  deviceTemperature = 0.0;
}

void MpptController::init() {
  pwmGenerator.init();
  deviceTemp.init();

  if(deviceTemp.isSensorFound()) {
    nectarError.clear_error(DEVICE_OVERHEAT);
  }
  
  nectarError.clear_error(NO_LOAD);
}

void MpptController::track() {
  static float old_power = 0.0;
  float moment_power;
  float dP;
  
  moment_power = data.moment_current * data.moment_voltage;
  data.moment_power = moment_power;
  dP = moment_power - old_power;
  
  if((moment_power < POWER_THRESHOLD) && (pwmGenerator.get_duty() > 0.5 )) {
    reset();
  } else {
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

float MpptController::getDeviceTemperature() {
  deviceTemperature = deviceTemp.getTemperature();
  if(deviceTemperature > DEVICE_TEMPERATURE_LIMIT_MAX) {
    if(!nectarError.has_error(DEVICE_OVERHEAT)) nectarError.set_error(DEVICE_OVERHEAT);
    printf("DEVICE OVERHEAT\r\n");
  } else {
    if(nectarError.has_error(DEVICE_OVERHEAT) && (deviceTemperature < (DEVICE_TEMPERATURE_LIMIT_MAX - 5.0))) nectarError.clear_error(DEVICE_OVERHEAT);
  }
  printf("[TEMPERATURE] internal %.2f\r\n", deviceTemperature);
  return deviceTemperature;
}

void MpptController::updateTemperatures() {
  if(deviceTemp.isNewValueAvailable()) data.device_temperature = getDeviceTemperature();
}

void MpptController::manualStartPwm() {
  pwmGenerator.start();
}

void MpptController::manualStopPwm() {
  pwmGenerator.stop();
}

void MpptController::manualIncreaseDuty(bool fineStep) {
  if(fineStep) {
    pwmGenerator.increase_duty(0.005);
  } else {
    pwmGenerator.increase_duty(PWM_STEP);
  }
}

void MpptController::manualDecreaseDuty(bool fineStep) {
  if(fineStep) {
    pwmGenerator.decrease_duty(0.005);
  } else {
    pwmGenerator.decrease_duty(PWM_STEP);
  }
}
