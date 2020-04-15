#include "CalibrateScreen.h"

#include "../../menu_service.h"
#include "../error/DebugErrorScreen.h"
#include "../01/ActiveStatusScreen.h"
#include "../00/ConfirmationScreen.h"

ScreenModel *CalibrateScreen::upModel() {
  return new DebugErrorScreen();
};

ScreenModel *CalibrateScreen::downModel() {
  return new ActiveStatusScreen();
};

ScreenModel *CalibrateScreen::nextModel() {
	return new ConfirmationScreen(Calibration, title + "?", true);
};