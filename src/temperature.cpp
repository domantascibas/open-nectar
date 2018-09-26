#include "consts.h"
#include "ErrorHandler.h"

// float MpptController::getDeviceTemperature() {
//   return deviceTemperature;
// }

/* Temperature sensor calibration value address */
#define TEMP110_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7C2))
#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7B8))
#define VDD_CALIB ((uint16_t) (330))
#define VDD_APPLI ((uint16_t) (300))

void initInternalTempSensor() {
  /* This code selects the HSI14 as clock source. */
  /* (1) Enable the peripheral clock of the ADC */
  /* (2) Start HSI14 RC oscillator */
  /* (3) Wait HSI14 is ready */
  /* (4) Select HSI14 by writing 00 in CKMODE (reset value) */
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; /* (1) */
  RCC->CR2 |= RCC_CR2_HSI14ON; /* (2) */
  while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0) /* (3) */
  {
  /* For robust implementation, add here time-out management */
  }
  //ADC1->CFGR2 &= (~ADC_CFGR2_CKMODE); /* (4) */

  /* (1) Select CHSEL16 for temperature sensor */
  /* (2) Select a sampling mode of 111 i.e. 239.5 ADC clk to be greater than
  17.1us */
  /* (3) Wake-up the Temperature sensor (only for VBAT, Temp sensor and
  VRefInt) */
  ADC1->CHSELR = ADC_CHSELR_CHSEL16; /* (1) */
  ADC1->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2; /* (2) */
  ADC1->CR |= ADC_CR_ADEN;
  ADC->CCR |= ADC_CCR_TSEN; /* (3) */
  
  printf("***\r\nInternal temp sensor initialized\r\n***\r\n");
}

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

// void MpptController::updateTemperatures() {
// //  if(data.safeToReadTemp) {
// //    data.safeToReadTemp = false;
   
// //    if(deviceTemp.isReadyToMeasure()) {
// //      deviceTemp.measureTemperature();
// //    }
   
// //    if(deviceTemp.isReadyToRead()) {
// //      deviceTemp.readTemperatureToStorage();
// //    }
   
// //    if(deviceTemp.isNewValueAvailable()) {
// // //      printf("[MPPT] 	 device temperature\r\n");
// //  //    deviceTemp.setNewValueNotAvailable();
// //      data.device_temperature = getDeviceTemperature();
// //      readInternalTempSensor();
// //    }
// //  }
// }