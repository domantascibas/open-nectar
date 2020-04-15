#include "ConfirmationScreen.h"

#include "../../menu_service.h"
#include "../00/SetLanguageScreen.h"
#include "../02/PairingScreen.h"
#include "../02/ResetScreen.h"
#include "../engineering_menu/CalibrateScreen.h"
#include "../error/DebugScreen.h"

ScreenModel *ConfirmationScreen::upModel() {
  return new ConfirmationScreen(type, title, !isYes);
};

ScreenModel *ConfirmationScreen::downModel() {
  return new ConfirmationScreen(type, title, !isYes);
};

ScreenModel *ConfirmationScreen::nextModel() {
  switch (type) {
  case Reset:
    if(isYes) {
      menu_actions::reset();
      return new SetLanguageScreen(localization::currentLanguage());
    } else {
      return new ResetScreen();
    }
  case Pairing:
    menu_actions::startPairing(isYes);
    return new PairingScreen();
	
	case Calibration:
		if(isYes) {
			menu_actions::calibratePowerBoard();
			return new DebugScreen();
		} else {
			return new CalibrateScreen();
		}
  }
};
