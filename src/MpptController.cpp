#include "consts.h"
#include "pwm_output.h"
#include "MpptController.h"
#include "ErrorHandler.h"
#include "data.h"

MpptController::MpptController() {
  last_increase = true;
  deviceTemperature = 0.0;
}

void MpptController::init() {
  pwm_output_init();
  nectarError.clear_error(DEVICE_OVERHEAT);
  nectarError.clear_error(NO_LOAD);
}

void MpptController::track() {
  static float old_power = 0.0;
  float moment_power;
  float dP;
  
  moment_power = data.moment_current * data.moment_voltage;
  data.moment_power = moment_power;
  dP = moment_power - old_power;
  
	if(data.moment_current < CURRENT_THRESHOLD) {
    reset();
    old_power = moment_power;
  } else {
    pwm_output_start();
    if(dP != 0) {
      if(dP > 0) {
        if(last_increase) {
          pwm_output_increase_duty();
          last_increase = true;
        } else {
          pwm_output_decrease_duty();
          last_increase = false;
        }
      } else {
        if(last_increase) {
          pwm_output_decrease_duty();
          last_increase = false;
        } else {
          pwm_output_increase_duty();
          last_increase = true;
        }
      }
      old_power = moment_power;
    } else {
      if(last_increase) {
        pwm_output_increase_duty();
        last_increase = true;
      } else {
        pwm_output_decrease_duty();
        last_increase = false;
      }
    }
  }
}

void MpptController::stop() {
  last_increase = true;
  pwm_output_stop();
  pwm_output_set_duty(PWM_DUTY_MIN);
  data.moment_power = 0;	
}

void MpptController::reset() {
  last_increase = true;
  pwm_output_stop();
  pwm_output_set_duty(PWM_DUTY_MIN);
  data.moment_power = 0;
  pwm_output_start();
}

void MpptController::swipe(float min, float max, float step) {
  static bool reverse = false;
  
  pwm_output_start();
  if(!reverse) {
    if(pwm_output_get_duty() >= max) {
      reverse = true;
    } else {
      pwm_output_increase_duty();
      reverse = false;
    }
  } else {
    if(pwm_output_get_duty() <= min) {
      reverse = false;
    } else {
      pwm_output_decrease_duty();
      reverse = true;
    }
  }
}

float MpptController::get_duty() {
  return pwm_output_get_duty();
}

bool MpptController::is_generator_on() {
  return pwm_output_is_on();
}

float MpptController::getDeviceTemperature() {
  return deviceTemperature;
}

/* Temperature sensor calibration value address */
#define TEMP110_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7C2))
#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7B8))
#define VDD_CALIB ((uint16_t) (330))
#define VDD_APPLI ((uint16_t) (300))

void readInternalTempSensor() {
  ADC1->CR |= ADC_CR_ADSTART;
  wait_us(30);
  int32_t temperature; /* will contain the temperature in degrees Celsius */
  temperature = (((int32_t) ADC1->DR * VDD_APPLI / VDD_CALIB) - (int32_t) *TEMP30_CAL_ADDR );
  temperature = temperature * (int32_t)(110 - 30);
  temperature = temperature / (int32_t)(*TEMP110_CAL_ADDR - *TEMP30_CAL_ADDR);
  temperature = temperature + 30;
  
  printf("processor temp: %d\r\n", temperature);
  if(temperature > PROCESSOR_INTERNAL_TEMPERATURE_LIMIT) {
    if(!nectarError.has_error(PROCESSOR_OVERHEAT)) nectarError.set_error(PROCESSOR_OVERHEAT);
    printf("PROCESSOR OVERHEAT\r\n");
  } else {
    if(nectarError.has_error(PROCESSOR_OVERHEAT) && (temperature < (PROCESSOR_INTERNAL_TEMPERATURE_LIMIT - 5.0))) nectarError.clear_error(PROCESSOR_OVERHEAT);
  }
}

void MpptController::updateTemperatures() {
//  if(data.safeToReadTemp) {
//    data.safeToReadTemp = false;
   
//    if(deviceTemp.isReadyToMeasure()) {
//      deviceTemp.measureTemperature();
//    }
   
//    if(deviceTemp.isReadyToRead()) {
//      deviceTemp.readTemperatureToStorage();
//    }
   
//    if(deviceTemp.isNewValueAvailable()) {
// //      printf("[MPPT] 	 device temperature\r\n");
//  //    deviceTemp.setNewValueNotAvailable();
//      data.device_temperature = getDeviceTemperature();
//      readInternalTempSensor();
//    }
//  }
}

void MpptController::manualStartPwm() {
  pwm_output_start();
}

void MpptController::manualStopPwm() {
  pwm_output_stop();
}

void MpptController::manualIncreaseDuty(bool fineStep) {
  if(fineStep) {
    pwm_output_increase_duty_fine();
  } else {
    pwm_output_increase_duty();
  }
}

void MpptController::manualDecreaseDuty(bool fineStep) {
  if(fineStep) {
    pwm_output_decrease_duty_fine();
  } else {
    pwm_output_decrease_duty();
  }
}
