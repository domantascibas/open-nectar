#include "ActiveTotalScreen.h"

#include "ActiveStatusScreen.h"
#include "DeviceModeScreen.h"
#include "SettingsScreen.h"

ScreenModel *ActiveTotalScreen::upModel() { return new ActiveStatusScreen(); };
ScreenModel *ActiveTotalScreen::downModel() {
  if(menu_actions::hasConfig()) return new SettingsScreen();
  else return new DeviceModeScreen();
};
