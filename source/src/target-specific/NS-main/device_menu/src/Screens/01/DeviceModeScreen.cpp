#include "DeviceModeScreen.h"

#include "../00/SetDeviceModeScreen.h"
#include "ActiveTotalScreen.h"
#include "SettingsScreen.h"

ScreenModel *DeviceModeScreen::upModel() { return new ActiveTotalScreen(); };
ScreenModel *DeviceModeScreen::downModel() { return new SettingsScreen(); };
ScreenModel *DeviceModeScreen::nextModel() {
    HeaterMode curr_mode = menu_actions::deviceMode();
    return new SetDeviceModeScreen(curr_mode);
};
