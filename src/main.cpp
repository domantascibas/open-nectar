#include "mbed.h"
#include "device_modes.h"
#include "main_board.h"
#include "data.h"
#include "service.h"
#include "ErrorHandler.h"

#define VERSION_MAJOR         1
#define VERSION_MINOR         0
#define VERSION_REVISION      2

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
int main() {  
  service::setup();
  initInternalTempSensor();
  
  printf("Starting device modes\r\n");
  device_modes::setup();
  DEBUG_PRINT("Energy Meters: %.4f, %.4f\r\n", data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
  main_board::setup();
  printf("SETUP DONE\r\n");
  
  while(1) {
    if(!nectarError.has_error(CALIBRATION_ERROR)) device_modes::loop();
    device_modes::calibrate_device();
    __WFI();
  }
  
  printf("Reached end of program\r\n");
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
