#include "mbed.h"
#include "pwm.h"
#include "device_modes.h"
#include "data.h"

static const PinName PWM = PB_1;
static const PinName SD = PB_0;
static const PinName MOSFET = PD_2;

static const float PWM_MIN = 0.1;
static const float PWM_MAX = 0.95;
static const float PWM_DUTY_STEP_CHANGE = 0.02;

PwmOut pwm_gen(PWM);
DigitalOut shutdown(SD);
DigitalIn mosfet_overheat(MOSFET);

namespace pwm {
  float clamp(float input, float min, float max) {
    if(input >= max) {
      return max;
    }
    if(input <= min) {
      return min;
    }
    return input;
  }

  void setup() {
    static const float frequency = 3.6;    //in kHz
    pwm_gen.period_us(1000/frequency);
    pwm_gen.write(data.pwm_duty);
    shutdown = DRIVER_OFF;
  }

  uint8_t adjust() {
    static float old_power = 0.0;
    static uint8_t duty_reduce_count;
    static bool last_increase = true;
    
    float pwm_duty = data.pwm_duty;
    float moment_power;
    float dP;
    
    shutdown = DRIVER_ON;
    moment_power = data.moment_current * data.moment_voltage;
    dP = moment_power - old_power;
    
    if(dP != 0) {
      if(dP > 0) {
        if(last_increase) {
          pwm_duty += PWM_DUTY_STEP_CHANGE;
          last_increase = true;
        } else {
          pwm_duty -= PWM_DUTY_STEP_CHANGE;
          last_increase = false;
        }
      } else {
        if(last_increase) {
          pwm_duty -= PWM_DUTY_STEP_CHANGE;
          last_increase = false;
        } else {
          pwm_duty += PWM_DUTY_STEP_CHANGE;
          last_increase = true;
        }
      }
      old_power = moment_power;
    }
    
    //TODO: else - follow last_increase
    
    pwm_duty = clamp(pwm_duty, PWM_MIN, PWM_MAX);
    pwm_gen.write(pwm_duty);
    data.pwm_duty = pwm_duty;
    return NS_OK;
  }

  void set() {
    if((data.moment_voltage >= VOLTAGE_LIMIT) || (data.moment_current >= CURRENT_LIMIT)) {
      shutdown = DRIVER_OFF;
    } else {
      shutdown = DRIVER_ON;
      pwm_gen.write(data.pwm_duty);
    }
  }
  
  void increase_duty() {
    data.pwm_duty += PWM_DUTY_STEP_CHANGE;
    data.pwm_duty = clamp(data.pwm_duty, PWM_MIN, PWM_MAX);
    printf("PWM DUTY: %.2f\r\n", data.pwm_duty);
  }
  
  void decrease_duty() {
    data.pwm_duty -= PWM_DUTY_STEP_CHANGE;
    data.pwm_duty = clamp(data.pwm_duty, PWM_MIN, PWM_MAX);
    printf("PWM DUTY: %.2f\r\n", data.pwm_duty);
  }

  void reset() {
    shutdown = DRIVER_OFF;
    data.pwm_duty = 0.1;
    pwm_gen.write(data.pwm_duty);
  }

  void swipe(float min, float max, float step) {
    static bool reverse = false;
    float pwm_duty = data.pwm_duty;
    shutdown = DRIVER_ON;
    if(!reverse) {
      if(pwm_duty >= max) {
        reverse = true;
      } else {
        pwm_duty += step;
        reverse = false;
      }
    } else {
      if(pwm_duty <= min) {
        reverse = false;
      } else {
        pwm_duty -= step;
        reverse = true;
      }
    }
    pwm_duty = clamp(pwm_duty, PWM_MIN, PWM_MAX);
    pwm_gen.write(pwm_duty);
    data.pwm_duty = pwm_duty;
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
