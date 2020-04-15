#include "ResetScreen.h"

#include "../../menu_service.h"
#include "../00/ConfirmationScreen.h"
#include "BackScreen.h"
#include "TemperatureScreen.h"
#include "LanguageScreen.h"

ScreenModel *ResetScreen::upModel() { return new BackScreen(); };

ScreenModel *ResetScreen::downModel() {
  if (menu_actions::hasConfig())
    return new LanguageScreen();
  return new TemperatureScreen(TemperatureDay);
};

ScreenModel *ResetScreen::nextModel() {
  return new ConfirmationScreen(Reset, title + "?", true);
};
