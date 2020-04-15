#include "ErrorScreen.h"

#include "../01/SettingsScreen.h"
#include "DebugScreen.h"

ScreenModel *ErrorScreen::upModel() {
    return new SettingsScreen();
};

ScreenModel *ErrorScreen::downModel() {
    return new DebugScreen();
};
