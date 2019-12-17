#include "mbed.h"
#include "error_handler.h"

ErrorHandler::ErrorHandler() {
  has_errors = false;
  last_error = NONE;
}

void ErrorHandler::set_error(ERROR_CODE err) {
  last_error = err;
  has_errors = true;
  raised_errors |= 1 << err;
  print_error(&err);
}

void ErrorHandler::get_errors(uint32_t *err) {
  if(raised_errors > 0x3FFF) raised_errors = 0x0;
  *err = raised_errors;
}

uint32_t ErrorHandler::get_errors() {
  if(raised_errors > 0x3FFF) raised_errors = 0x0;
  return raised_errors;
}

void ErrorHandler::save_error_code(uint32_t err) {
  raised_errors = err;
}

void ErrorHandler::get_last_error(ERROR_CODE *err) {
  *err = last_error;
}

void ErrorHandler::clear_error() {
  last_error = NONE;
  raised_errors = 0x00000000;
  has_errors = false;
  printf("ERRORS CLEARED ALL\n");
}

void ErrorHandler::clear_error(ERROR_CODE err) {
  raised_errors &= ~(1 << err);
  printf("ERRORS CLEARED %X\n", err);
  if(raised_errors == 0x00000000) {
    last_error = NONE;
    has_errors = false;
    printf("ERRORS CLEARED ALL\n");
  } else {
    printf("ERRORS PENDING %X\n", (unsigned int)raised_errors);
  }
}

bool ErrorHandler::has_error(ERROR_CODE err) {
  return (raised_errors & (1 << err)) >> err;
}

void ErrorHandler::print_error(ERROR_CODE *err) {
  switch(*err) {
    case FLASH_ACCESS_ERROR:
      printf("ERROR ERR FLASH ACCESS ERROR\n");
      break;
    
    case ADC_VOLTAGE_ERROR:
      printf("ERROR ERR ADC VOLTAGE ERROR\n");
      break;
    
    case ADC_CURRENT_ERROR:
      printf("ERROR ERR ADC CURRENT ERROR\n");
      break;
    
    case CALIBRATION_ERROR:
      printf("ERROR ERR CALIBRATION ERROR\n");
      break;
    
    case DC_OVER_VOLTAGE:
      printf("ERROR ERR DC OVER VOLTAGE\n");
      break;
    
    case DC_OVER_CURRENT:
      printf("ERROR ERR DC OVER CURRENT\n");
      break;
    
    case DEVICE_OVERHEAT:
      printf("ERROR ERR DEVICE OVERHEAT\n");
      break;
    
    case NO_LOAD:
      printf("ERROR ERR NO LOAD\n");
      break;
    
    case NO_BOILER_TEMP:
      printf("ERROR ERR NO BOILER TEMPERATURE\n");
      break;
    
    case MAX_TEMPERATURE:
      printf("ERROR ERR REACHED MAX TEMPERATURE\n");
      break;
    
    case MIN_TEMPERATURE:
      printf("ERROR ERR REACHED MIN TEMPERATURE\n");
      break;
    
    case PROCESSOR_OVERHEAT:
      printf("ERROR ERR PROCESSOR OVERHEAT\n");
      break;
    
    default:
      printf("ERROR ERR Unspecified Error\n");
      break;
  }
}
