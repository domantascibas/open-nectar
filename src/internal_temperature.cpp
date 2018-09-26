#include "consts.h"
#include "internal_temperature.h"

void internal_temperature_init(void) {
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  RCC->CR2 |= RCC_CR2_HSI14ON;
  while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0) {
  /* For robust implementation, add here time-out management */
  }
  
  ADC1->CHSELR = ADC_CHSELR_CHSEL16;
  ADC1->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2;
  ADC1->CR |= ADC_CR_ADEN;
  ADC->CCR |= ADC_CCR_TSEN;
  
  printf("***\r\nInternal temp sensor initialized\r\n***\r\n");
}

float internal_temperature_measure(void) {
  ADC1->CR |= ADC_CR_ADSTART;
  wait_us(30);
  int32_t temperature; /* will contain the temperature in degrees Celsius */
  temperature = (((int32_t) ADC1->DR * VDD_APPLI / VDD_CALIB) - (int32_t) *TEMP30_CAL_ADDR );
  temperature = temperature * (int32_t)(110 - 30);
  temperature = temperature / (int32_t)(*TEMP110_CAL_ADDR - *TEMP30_CAL_ADDR);
  temperature = temperature + 30;
  return temperature;
}

// void readInternalTempSensor() {
//   ADC1->CR |= ADC_CR_ADSTART;
//   wait_us(30);
//   int32_t temperature; /* will contain the temperature in degrees Celsius */
//   temperature = (((int32_t) ADC1->DR * VDD_APPLI / VDD_CALIB) - (int32_t) *TEMP30_CAL_ADDR );
//   temperature = temperature * (int32_t)(110 - 30);
//   temperature = temperature / (int32_t)(*TEMP110_CAL_ADDR - *TEMP30_CAL_ADDR);
//   temperature = temperature + 30;
  
//   printf("processor temp: %d\r\n", temperature);
//   if(temperature > PROCESSOR_INTERNAL_TEMPERATURE_LIMIT) {
//     if(!nectarError.has_error(PROCESSOR_OVERHEAT)) nectarError.set_error(PROCESSOR_OVERHEAT);
//     printf("PROCESSOR OVERHEAT\r\n");
//   } else {
//     if(nectarError.has_error(PROCESSOR_OVERHEAT) && (temperature < (PROCESSOR_INTERNAL_TEMPERATURE_LIMIT - 5.0))) nectarError.clear_error(PROCESSOR_OVERHEAT);
//   }
// }

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