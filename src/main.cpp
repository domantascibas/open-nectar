#include "mbed.h"
#include "storage.h"
#include "device_modes.h"
#include "main_board.h"
#include "data.h"
#include "service.h"
#include "ErrorHandler.h"

DigitalOut pa0(PA_0);
DigitalOut pa1(PA_1);
DigitalOut pa2(PA_2);
DigitalOut pa3(PA_3);
DigitalOut pa4(PA_4);
DigitalOut pa5(PA_5);
DigitalOut pa6(PA_6);
DigitalOut pa7(PA_7);
DigitalOut pa8(PA_8);
DigitalOut pa9(PA_9);
DigitalOut pa10(PA_10);
DigitalOut pa11(PA_11);
DigitalOut pa12(PA_12);
//DigitalOut pa13(PA_13);
//DigitalOut pa14(PA_14);
//DigitalOut pa15(PA_15);

DigitalOut pb0(PB_0);
DigitalOut pb1(PB_1);
DigitalOut pb2(PB_2);
DigitalOut pb3(PB_3);
DigitalOut pb4(PB_4);
DigitalOut pb5(PB_5);
DigitalOut pb6(PB_6);
DigitalOut pb7(PB_7);
DigitalOut pb8(PB_8);
DigitalOut pb9(PB_9);
DigitalOut pb10(PB_10);
DigitalOut pb11(PB_11);
DigitalOut pb12(PB_12);
//DigitalOut pb13(PB_13);
//DigitalOut pb14(PB_14);
DigitalOut pb15(PB_15);

DigitalOut pc0(PC_0);
DigitalOut pc1(PC_1);
DigitalOut pc2(PC_2);
DigitalOut pc3(PC_3);
DigitalOut pc4(PC_4);
DigitalOut pc5(PC_5);
DigitalOut pc6(PC_6);
//DigitalOut pc7(PC_7);
DigitalOut pc8(PC_8);
//DigitalOut pc9(PC_9);
//DigitalOut pc10(PC_10);
//DigitalOut pc11(PC_11);
//DigitalOut pc12(PC_12);
DigitalOut pc13(PC_13);
DigitalOut pc14(PC_14);
DigitalOut pc15(PC_15);

//DigitalOut pd2(PD_2);

DigitalOut pf0(PF_0);
DigitalOut pf1(PF_1);
DigitalOut pf11(PF_11);

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

void initIndependentWatchdog() {
  /* (1) Activate IWDG (not needed if done in option bytes) */
  /* (2) Enable write access to IWDG registers */
  /* (3) Set prescaler by 8 */
  /* (4) Set reload value to have a rollover each 100ms */
  /* (5) Check if flags are reset */
  /* (6) Refresh counter */
  IWDG->KR = 0xCCCC; /* (1) */
  IWDG->KR = 0x5555; /* (2) */
  IWDG->PR = IWDG_PR_PR_0; /* (3) */
  IWDG->RLR = 0xFFF; /* (4) */
  while (IWDG->SR) /* (5) */
  {
  /* add time out here for a robust application */
  }
  IWDG->KR = 0xAAAA; /* (6) */
}

void kickTheDog() {
  IWDG->KR = 0xAAAA;
//  printf("kick the dog\r\n");
}

int main() {
  service::setup();
	Storage::init();
  initInternalTempSensor();
  
  printf("Starting device modes\r\n");
  device_modes::setup();
  DEBUG_PRINT("Energy Meters: %.4f, %.4f\r\n", data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
  main_board::setup();
  printf("SETUP DONE\r\n");
  
  initIndependentWatchdog();
  
  while(1) {
    kickTheDog();
    if(!nectarError.has_error(CALIBRATION_ERROR)) device_modes::loop();
    device_modes::calibrate_device();
    __WFI();
  }
  
  printf("Reached end of program\r\n");
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
