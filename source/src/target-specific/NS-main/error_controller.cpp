#include "consts.h"
#include "pins.h"
#include "error_controller.h"

ErrorHandler mainBoardError;
ErrorHandler powerBoardError;

void error_controller_init(void) {
	mainBoardError.set_error(FLASH_ACCESS_ERROR);
//	mainBoardError.set_error(MAX_TEMPERATURE);
//	mainBoardError.set_error(MIN_TEMPERATURE);
	mainBoardError.set_error(NO_BOILER_TEMP);
//	mainBoardError.set_error(PROCESSOR_OVERHEAT);
}
