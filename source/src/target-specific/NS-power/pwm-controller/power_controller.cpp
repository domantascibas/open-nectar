#include "consts.h"
#include "pins.h"
#include "pwm_controller.h"
#include "power_controller.h"
// #include "error_controller.h"
#include "error_handler.h"

extern "C" {
    #include "data.h"
}

void generator_on(void);
void generator_off(void);

static bool output = false;
static bool last_increase = true;

DigitalOut shutdown(SD_PIN);

void power_controller_init(void) {
  generator_off();
  error_clear(NS_DEVICE_OVERHEAT);
  error_clear(NS_NO_LOAD);
}

void power_controller_mppt_stop(void) {
  last_increase = true;
  generator_off();
  pwm_controller_set_duty(PWM_DUTY_MIN);
  uint32_t p = 0;
  PowerData_write(M_POWER, &p);
}

void power_controller_mppt_start(void) {
  static float old_power = 0.0;
  float moment_power;
  float dP;
  uint16_t iv;
  
  // moment_power = data.moment_current * data.moment_voltage;
  moment_power = PowerData_calculatePower();
  // data.moment_power = moment_power;
  dP = moment_power - old_power;
  
  PowerData_read(M_CURRENT, &iv);
	if(DIV_VI_CONVERT(iv) < CURRENT_THRESHOLD) {
    power_controller_mppt_restart();
    old_power = moment_power;
  } else {
    generator_on();
    if(dP != 0) {
      if(dP > 0) {
        if(last_increase) {
          pwm_controller_increase_duty(PWM_STEP);
          last_increase = true;
        } else {
          pwm_controller_decrease_duty(PWM_STEP);
          last_increase = false;
        }
      } else {
        if(last_increase) {
          pwm_controller_decrease_duty(PWM_STEP);
          last_increase = false;
        } else {
          pwm_controller_increase_duty(PWM_STEP);
          last_increase = true;
        }
      }
      old_power = moment_power;
    } else {
      if(last_increase) {
        pwm_controller_increase_duty(PWM_STEP);
        last_increase = true;
      } else {
        pwm_controller_decrease_duty(PWM_STEP);
        last_increase = false;
      }
    }
  }
}

void power_controller_mppt_restart(void) {
  power_controller_mppt_stop();
  generator_on();
}

void power_controller_swipe() {
  static bool reverse = false;
  
  generator_on();
  if(!reverse) {
    if(pwm_controller_get_duty() >= PWM_DUTY_MAX) {
      reverse = true;
    } else {
      pwm_controller_increase_duty(PWM_STEP);
      reverse = false;
    }
  } else {
    if(pwm_controller_get_duty() <= PWM_DUTY_MIN) {
      reverse = false;
    } else {
      pwm_controller_decrease_duty(PWM_STEP);
      reverse = true;
    }
  }
}

float power_controller_get_duty() {
  return pwm_controller_get_duty();
}

bool power_controller_is_generator_on() {
  return output;
}

void power_controller_manual_increase_duty(bool fine) {
  if(fine) {
    pwm_controller_increase_duty(PWM_STEP_FINE);
  } else {
    pwm_controller_increase_duty(PWM_STEP);
  }
}

void power_controller_manual_decrease_duty(bool fine) {
  if(fine) {
    pwm_controller_decrease_duty(PWM_STEP_FINE);
  } else {
    pwm_controller_decrease_duty(PWM_STEP);
  }
}

void power_controller_manual_output_on(void) {
  generator_on();
}

void power_controller_manual_output_off(void) {
  generator_off();
}

void generator_on(void) {
  output = true;
  shutdown = DRIVER_ON;
}

void generator_off(void) {
  output = false;
  shutdown = DRIVER_OFF;
}
