#include "TemperatureController.h"
#include "ErrorHandler.h"
#include "DataService.h"
#include "device_modes.h"
#include "ServiceComms.h"

static const PinName BOILER_TEMP_PROBE = PB_8;

static const float WATER_TEMPERATURE_LIMIT_MIN = 5.0;
static const float WATER_TEMPERATURE_LIMIT_MAX = 90.0;

TemperatureSensor boilerTemp(BOILER_TEMP_PROBE, 3);

void TemperatureController::init() {
  boilerTemp.init();
  
  if(boilerTemp.isSensorFound()) {
    mainBoardError.clear_error(NO_BOILER_TEMP);
  }
}

float TemperatureController::getBoilerTemperature() {
  boilerTemperature = boilerTemp.getTemperature();
  if(boilerTemperature > WATER_TEMPERATURE_LIMIT_MAX) {
    mainBoardError.set_error(MAX_TEMPERATURE);
  } else if(boilerTemperature < WATER_TEMPERATURE_LIMIT_MIN) {
    mainBoardError.set_error(MIN_TEMPERATURE);
  } else {
    if(mainBoardError.has_error(MAX_TEMPERATURE)) mainBoardError.clear_error(MAX_TEMPERATURE);
    if(mainBoardError.has_error(MIN_TEMPERATURE)) mainBoardError.clear_error(MIN_TEMPERATURE);
    printf("[TEMPERATURE] boiler %.2f\r\n", boilerTemperature);
  }
  return boilerTemperature;
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
}

void TemperatureController::updateTemperatures() {
  if(boilerTemp.isNewValueAvailable() || service::isNewValueAvailable()) {
    readInternalTempSensor();
    if(deviceOpMode.isInTestStand()) {
      temperatureData.setBoilerTemperature(service::getFakeTemperature());
      printf("new fake temp\r\n");
    } else {
      temperatureData.setBoilerTemperature(getBoilerTemperature());
    }
    device_modes::updateHeaterMode = true;
  }
}
