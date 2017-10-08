#include "error_handler.h"

ErrorHandler NectarError;

ErrorHandler::ErrorHandler()
  : has_errors(false), last_error(NONE), raised_errors(0x00000000) {
}

void ErrorHandler::set_error(ERROR_CODE err) {
  last_error = err;
  has_errors = true;
  raised_errors |= 1 << err;
  //TODO printf error to serial
}

void ErrorHandler::get_errors(uint32_t *err) {
  *err = raised_errors;
}

void ErrorHandler::get_last_error(ERROR_CODE *err) {
  *err = last_error;
}

void ErrorHandler::clear_error() {
  last_error = NONE;
  raised_errors = 0x00000000;
  has_errors = false;
}

void ErrorHandler::clear_error(ERROR_CODE err) {
  raised_errors &= ~(1 << err);
  if(raised_errors == 0x00000000) {
    last_error = NONE;
    has_errors = false;
  }
}
