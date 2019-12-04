#include "consts.h"
#include "error_controller.h"
#include <stdint.h>

static ErrorHandler nectarError;

void error_controller_init(void) {
    nectarError.set_error(FLASH_ACCESS_ERROR);
    nectarError.set_error(ADC_VOLTAGE_ERROR);
    nectarError.set_error(ADC_CURRENT_ERROR);
    nectarError.set_error(CALIBRATION_ERROR);
    nectarError.set_error(DC_OVER_VOLTAGE);
    nectarError.set_error(DC_OVER_CURRENT);
    nectarError.set_error(DC_CURRENT_LEAKS);
    nectarError.set_error(NO_LOAD);
    nectarError.set_error(DEVICE_OVERHEAT);
    nectarError.set_error(PROCESSOR_OVERHEAT);
}

void nectarError_clear_error(ERROR_CODE code) {
    nectarError.clear_error(code);
}

void nectarError_set_error(ERROR_CODE code) {
    nectarError.set_error(code);
}

uint8_t nectarError_has_error(ERROR_CODE code) {
    return (uint8_t)nectarError.has_error(code);
}

uint32_t nectarError_get_errors() {
    return nectarError.get_errors();
}

uint8_t nectarError_has_errors(void) {
    return nectarError.has_errors;
}
