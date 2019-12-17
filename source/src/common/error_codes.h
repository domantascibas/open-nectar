#ifndef NS_ERROR_CODES_H
#define NS_ERROR_CODES_H

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
