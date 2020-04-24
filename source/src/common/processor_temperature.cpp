#include "mbed.h"
#include "processor_temperature.h"
#include "datastore.h"

/* Temperature sensor calibration value address */
#define TEMP110_CAL_ADDR ((uint16_t *)((uint32_t)0x1FFFF7C2))
#define TEMP30_CAL_ADDR ((uint16_t *)((uint32_t)0x1FFFF7B8))
#define VDD_CALIB ((uint16_t)(330))
#define VDD_APPLI ((uint16_t)(300))

static uint16_t procTempInterval = 3000;

static void processor_temperature_init(void);
static void processor_temperature_measure(void);

void procTempFunc(void) {
    processor_temperature_init();

    while (1) {
        processor_temperature_measure();
        printf("new temp!! %d\r\n", datastore.sBoard.sTemperature.ucProcessor);
        ThisThread::sleep_for(procTempInterval);
    }
}

void procTempIntervalSet(uint8_t) {
}

void procTempIntervalGet(uint8_t interval) {

}

static void processor_temperature_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->CR2 |= RCC_CR2_HSI14ON;
    while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0) {
        /* For robust implementation, add here time-out management */
    }

    ADC1->CHSELR = ADC_CHSELR_CHSEL16;
    ADC1->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2;
    ADC1->CR |= ADC_CR_ADEN;
    ADC->CCR |= ADC_CCR_TSEN;
    printf("[init] processor temp\r\n");
}

static void processor_temperature_measure(void) {
    ADC1->CR |= ADC_CR_ADSTART;
    wait_us(50);
    int32_t temperature; /* will contain the temperature in degrees Celsius */
    temperature = (((int32_t) ADC1->DR * VDD_APPLI / VDD_CALIB) - (int32_t) * TEMP30_CAL_ADDR);
    temperature = temperature * (int32_t)(110 - 30);
    temperature = temperature / (int32_t)(*TEMP110_CAL_ADDR - *TEMP30_CAL_ADDR);
    temperature = temperature + 30;
    datastore.sBoard.sTemperature.ucProcessor = (uint8_t)temperature;
}
