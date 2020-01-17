#ifndef NS_ERROR_CODES_H
#define NS_ERROR_CODES_H

typedef enum {
    NS_FLASH_ACCESS_MAIN,
    NS_FLASH_ACCESS_POWER,
    NS_ADC_VOLTAGE,
    NS_ADC_CURRENT,
    NS_CALIBRATION,
    NS_DC_LOW_VOLTAGE,
    NS_DC_OVER_VOLTAGE,
    NS_DC_OVER_CURRENT,
    NS_DC_CURRENT_LEAKS,
    NS_NO_LOAD,
    NS_NO_BOILER_TEMP,
    NS_DEVICE_OVERHEAT,
    NS_MAX_TEMPERATURE,
    NS_MIN_TEMPERATURE,
    NS_PROCESSOR_OVERHEAT_MAIN,
    NS_PROCESSOR_OVERHEAT_POWER,
    NS_ERROR_COUNT
} NS_ERROR;

typedef enum {
    FLASH_ACCESS_ERROR,
    ADC_VOLTAGE_ERROR,
    ADC_CURRENT_ERROR,
    CALIBRATION_ERROR,
    DC_LOW_VOLTAGE,
    DC_OVER_VOLTAGE,
    DC_OVER_CURRENT,
    DC_CURRENT_LEAKS,
    NO_LOAD,
    NO_BOILER_TEMP,
    DEVICE_OVERHEAT,
    MAX_TEMPERATURE,
    MIN_TEMPERATURE,
    PROCESSOR_OVERHEAT,
    NONE = 0x80000000
} ERROR_CODE;

#endif
