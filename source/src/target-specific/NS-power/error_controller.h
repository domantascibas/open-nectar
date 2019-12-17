#ifndef _NS_ERROR_CONTROLLER_H
#define _NS_ERROR_CONTROLLER_H

#include "error_codes.h"

void error_controller_init(void);
void nectarError_clear_error(ERROR_CODE code);
void nectarError_set_error(ERROR_CODE code);
uint8_t nectarError_has_error(ERROR_CODE code);
uint32_t nectarError_get_errors();
uint8_t nectarError_has_errors(void);

#endif
