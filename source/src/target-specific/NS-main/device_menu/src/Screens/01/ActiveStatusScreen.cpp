#include "ActiveStatusScreen.h"

#include "../../menu_service.h"
#include "../engineering_menu/CalibrateScreen.h"
#include "ActiveTotalScreen.h"
#include "SettingsScreen.h"

ScreenModel *ActiveStatusScreen::upModel() {
    if (menu_actions::isInTestMode()) {
        return new CalibrateScreen();
    } else {
        return new SettingsScreen();
    }
};

ScreenModel *ActiveStatusScreen::downModel() {
    return new ActiveTotalScreen();
};
