#include "BackScreen.h"

#include "../../menu_service.h"
#include "../01/SettingsScreen.h"
#include "../02/PairingScreen.h"
#include "LanguageScreen.h"
#include "ResetScreen.h"

ScreenModel *BackScreen::upModel() {
  if (menu_actions::hasConfig()) {
    return new LanguageScreen();
  } else {
    return new PairingScreen();
  }
};

ScreenModel *BackScreen::downModel() {
  return new ResetScreen();
};

ScreenModel *BackScreen::nextModel() {
  return new SettingsScreen();
};
