#include "SetDeviceModeScreen.h"

#include "../01/ActiveStatusScreen.h"
#include "../01/DeviceModeScreen.h"

ScreenModel *SetDeviceModeScreen::upModel() {
	if(menu_actions::isInTestMode()) {
		return new SetDeviceModeScreen(
      (HeaterMode)(mode - 1 < 0 ? Count - 1 : mode - 1));
	} else {
		return new SetDeviceModeScreen(
      (HeaterMode)(mode - 1 < 0 ? Count - 2 : mode - 1));
	}
};

ScreenModel *SetDeviceModeScreen::downModel() {
	if(menu_actions::isInTestMode()) {
		return new SetDeviceModeScreen(
      (HeaterMode)(mode + 1 >= Count ? 0 : mode + 1));
	} else {
		return new SetDeviceModeScreen(
      (HeaterMode)(mode + 1 >= Count - 1 ? 0 : mode + 1));
	}
};

ScreenModel *SetDeviceModeScreen::nextModel() {
  menu_actions::setDeviceMode(mode);
	menu_actions::modeSelected();
  return new DeviceModeScreen();
};
