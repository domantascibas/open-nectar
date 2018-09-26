#include "consts.h"
#include "MpptController.h"
#include "PwmController.h"
#include "ErrorHandler.h"
#include "data.h"

PwmController pwmGenerator(1.8, 0.1, 0.95);
//TemperatureSensor deviceTemp(DEVICE_TEMP_PROBE, 5);

MpptController::MpptController() {
  last_increase = true;
  deviceTemperature = 0.0;
}

void MpptController::init() {
  pwmGenerator.init();
//  deviceTemp.init();

//  if(deviceTemp.isSensorFound()) {
    nectarError.clear_error(DEVICE_OVERHEAT);
//  }
  
  nectarError.clear_error(NO_LOAD);
}

void MpptController::track() {
  static float old_power = 0.0;
  float moment_power;
  float dP;
  
  moment_power = data.moment_current * data.moment_voltage;
  data.moment_power = moment_power;
  dP = moment_power - old_power;
  
//  if((data.moment_current < CURRENT_THRESHOLD) && (pwmGenerator.get_duty() >= 0.94 )) {
	if(data.moment_current < CURRENT_THRESHOLD) {
    reset();
    old_power = moment_power;
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

void MpptController::stop() {
  last_increase = true;
  pwmGenerator.stop();
  pwmGenerator.set_duty(0.1);
  data.moment_power = 0;	
}

void MpptController::reset() {
  last_increase = true;
  pwmGenerator.stop();
  pwmGenerator.set_duty(0.1);
  data.moment_power = 0;
	pwmGenerator.start();
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
//  deviceTemperature = deviceTemp.getTemperature();
//  if(deviceTemperature > DEVICE_TEMPERATURE_LIMIT_MAX) {
//    if(!nectarError.has_error(DEVICE_OVERHEAT)) nectarError.set_error(DEVICE_OVERHEAT);
//    printf("DEVICE OVERHEAT\r\n");
//  } else {
//    if(nectarError.has_error(DEVICE_OVERHEAT) && (deviceTemperature < (DEVICE_TEMPERATURE_LIMIT_MAX - 5.0))) nectarError.clear_error(DEVICE_OVERHEAT);
//  }
// //  printf("[TEMPERATURE] internal %.2f\r\n", deviceTemperature);
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
