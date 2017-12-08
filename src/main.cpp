#include "mbed.h"
#include "ServiceComms.h"
#include "EspComms.h"
#include "PowerBoardComms.h"
#include "device_modes.h"
#include "menu_service.h"
#include "ErrorHandler.h"
#include "TemperatureController.h"
#include "DataService.h"

#define VERSION_MAJOR         1
#define VERSION_MINOR         0
#define VERSION_REVISION      1

bool inErrorScreen = false;

TemperatureController tempController;

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
  static bool isFirst = true;
  mainBoardError.save_error_code(0x200);
  service::setup();
  
  initInternalTempSensor();
  
  menu_service::setup();
  wait(1.0);
  tempController.init();
  wait(1.0);
  tempController.updateTemperatures();
  power_board::setup();
  esp::setup();
  device_modes::setup();

  while(1) {
    while(!power_board::hasReceivedFirstMessage()) {
      __WFI();
    }
    
    if(isFirst) {
      isFirst = false;
      menu_service::needUpdate = true;
    }
    
//    if((mainBoardError.has_errors || powerBoardError.has_errors) && !inErrorScreen) {
//      printf("[ERROR] present\r\n");
//      menu_service::needUpdate = true;
//      inErrorScreen = true;
//    } else if((!mainBoardError.has_errors && !powerBoardError.has_errors) && inErrorScreen) {
//      menu_service::needUpdate = true;
//      inErrorScreen = false;
//    }
    
    if(menu_service::needUpdate) {
      menu_service::updateScreen();
//      printf("Available memory = %d\r\n\n", Memory::availableMemory(1) );
    }
    
    switch(deviceOpMode.getCurrentMode()) {
      default:
      case NOT_CONFIGURED:
        if(espData.is_configured) {
          printf("NO CONFIG -> HAS CONFIG\r\n");
          deviceOpMode.setConfigured();
          menu_service::needUpdate = true;
          menu_service::resetScreen = true;
        }
        break;
      
      case CONFIGURED:
        if(!espData.is_configured) {
          printf("HAS CONFIG -> NO CONFIG\r\n");
        }
        break;
      
      case WELCOME:
        //nothing to do here
        //wait for user to finish onboarding
        break;
      
      case TEST_MODE:
        if(isFirst) {
          isFirst = false;
          deviceOpMode.setTestMode();
        }
        break;
    }
    
    tempController.updateTemperatures();
    if(!deviceOpMode.isOnboarding()) {
      device_modes::loop();
    }
    __WFI();
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
