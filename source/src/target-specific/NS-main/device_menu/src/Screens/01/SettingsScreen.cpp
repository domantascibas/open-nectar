#include "SettingsScreen.h"

#include "../02/LanguageScreen.h"
#include "../02/TemperatureScreen.h"
#include "../error/DebugScreen.h"
#include "../error/ErrorScreen.h"
#include "ActiveStatusScreen.h"
#include "DeviceModeScreen.h"
#include "ActiveTotalScreen.h"

#include "../../menu_service.h"

ScreenModel *SettingsScreen::upModel() {
    if (menu_actions::hasConfig()) return new ActiveTotalScreen();
    else return new DeviceModeScreen();
};

ScreenModel *SettingsScreen::downModel() {
    if (menu_actions::isInTestMode()) {
        if (menu_actions::hasErrors()) {
            return new ErrorScreen();
        } else {
            return new DebugScreen();
        }
    } else {
        return new ActiveStatusScreen();
    }
};

ScreenModel *SettingsScreen::nextModel() {
    if (menu_actions::hasConfig())
        return new LanguageScreen();
    return new TemperatureScreen(TemperatureDay);
};
