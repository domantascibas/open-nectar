#ifndef _NS_ERROR_CONTROLLER_H
#define _NS_ERROR_CONTROLLER_H

#include "error_handler.h"
#include "error_codes.h"

void error_controller_init(void);

extern ErrorHandler mainBoardError;
extern ErrorHandler powerBoardError;

#endif
