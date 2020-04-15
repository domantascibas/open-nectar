#include "DebugScreen.h"

#include "DebugErrorScreen.h"
#include "ErrorScreen.h"
#include "../01/SettingsScreen.h"

ScreenModel *DebugScreen::upModel() {
  if(menu_actions::hasErrors()) {
    return new ErrorScreen();
  } else {
    return new SettingsScreen();
  }
};

ScreenModel *DebugScreen::downModel() {
  return new DebugErrorScreen();
};
