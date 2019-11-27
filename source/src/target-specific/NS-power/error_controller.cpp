#include "consts.h"
#include "error_controller.h"

ErrorHandler nectarError;

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