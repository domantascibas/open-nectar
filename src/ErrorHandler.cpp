#include "ErrorHandler.h"

ErrorHandler mainBoardError;
ErrorHandler powerBoardError;

ErrorHandler::ErrorHandler() {
  raised_errors = 0x00000000;
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
  *err = raised_errors;
}

uint32_t ErrorHandler::get_errors() {
//  printf("ERRORS %d\r\n", raised_errors);
  return raised_errors;
}

void ErrorHandler::save_error_code(uint32_t err) {
  raised_errors = err;
  if(raised_errors != 0x00000000) has_errors = true;
  else has_errors = false;
//  printf("SAVE ERROR %d\r\n", raised_errors);
}

void ErrorHandler::get_last_error(ERROR_CODE *err) {
  *err = last_error;
}

void ErrorHandler::clear_error() {
  last_error = NONE;
  raised_errors = 0x00000000;
  has_errors = false;
  printf("[OK] All errors cleared!\r\n");
}

void ErrorHandler::clear_error(ERROR_CODE err) {
  raised_errors &= ~(1 << err);
  printf("[OK] Error 0x%X cleared\r\n", err);
  if(raised_errors == 0x00000000) {
    last_error = NONE;
    has_errors = false;
    printf("[OK] All errors cleared!\r\n");
  } else {
    printf("[WARNING] Errors pending: 0x%X\r\n", raised_errors);
  }
}

bool ErrorHandler::has_error(ERROR_CODE err) {
  return (raised_errors & (1 << err)) >> err;
}

void ErrorHandler::print_error(ERROR_CODE *err) {
  switch(*err) {
    case FLASH_ACCESS_ERROR:
      printf("[ERROR] FLASH ACCESS ERROR\r\n");
      break;
    
    case ADC_VOLTAGE_ERROR:
      printf("[ERROR] ADC VOLTAGE ERROR\r\n");
      break;
    
    case ADC_CURRENT_ERROR:
      printf("[ERROR] ADC CURRENT ERROR\r\n");
      break;
    
    case CALIBRATION_ERROR:
      printf("[ERROR] CALIBRATION ERROR\r\n");
      break;
    
    case DC_LOW_VOLTAGE:
      printf("[ERROR] DC LOW VOLTAGE\r\n");
      break;
    
    case DC_OVER_VOLTAGE:
      printf("[ERROR] DC OVER VOLTAGE\r\n");
      break;
    
    case DC_OVER_CURRENT:
      printf("[ERROR] DC OVER CURRENT\r\n");
      break;
    
    case DEVICE_OVERHEAT:
      printf("[ERROR] DEVICE OVERHEAT\r\n");
      break;
    
    case NO_LOAD:
      printf("[ERROR] NO LOAD\r\n");
      break;
    
    case NO_BOILER_TEMP:
      printf("[ERROR] NO BOILER TEMPERATURE\r\n");
      break;
    
    case MAX_TEMPERATURE:
      printf("[ERROR] REACHED MAX TEMPERATURE\r\n");
      break;
    
    case MIN_TEMPERATURE:
      printf("[ERROR] REACHED MIN TEMPERATURE\r\n");
      break;
    
    default:
      printf("[ERROR] Unspecified error\r\n");
      break;
  }
}
