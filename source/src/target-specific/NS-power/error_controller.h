#ifndef _NS_ERROR_CONTROLLER_H
#define _NS_ERROR_CONTROLLER_H

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

// extern "C" {
void error_controller_init(void);
void nectarError_clear_error(ERROR_CODE code);
void nectarError_set_error(ERROR_CODE code);
uint8_t nectarError_has_error(ERROR_CODE code);
uint32_t nectarError_get_errors();
uint8_t nectarError_has_errors(void);
// }

#endif
