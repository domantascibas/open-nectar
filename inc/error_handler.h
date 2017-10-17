#include <stdint.h>

#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

typedef enum {
  FLASH_ACCESS_ERROR,
  ADC_VOLTAGE_ERROR,
  ADC_CURRENT_ERROR,
  CALIBRATION_ERROR,
  DC_OVER_VOLTAGE,
  DC_OVER_CURRENT,
  DC_CURRENT_LEAKS,
  DEVICE_OVERHEAT,
  MOSFET_OVERHEAT,
  NO_LOAD,
  NO_BOILER_TEMP,
  MAX_TEMPERATURE,
  MIN_TEMPERATURE,
  NONE = 0x80000000
} ERROR_CODE;

struct ErrorHandler {
  ErrorHandler();
  public:
    void set_error(ERROR_CODE);
    void get_errors(uint32_t *);
    void get_last_error(ERROR_CODE *);
    void clear_error();
    void clear_error(ERROR_CODE);
    bool has_error(ERROR_CODE);
    bool has_errors;
    
  private:
    uint32_t raised_errors;
    ERROR_CODE last_error;
    void print_error(ERROR_CODE *);
};

extern ErrorHandler MainBoardError;
extern ErrorHandler PowerBoardError;

#endif
