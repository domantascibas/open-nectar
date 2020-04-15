#include "LanguageScreen.h"

#include "../../menu_service.h"
#include "../00/SetLanguageScreen.h"
#include "../02/PairingScreen.h"
#include "BackScreen.h"
#include "TimeScreen.h"

#include "ResetScreen.h"

ScreenModel *LanguageScreen::upModel() {
    if (menu_actions::hasConfig())
        return new ResetScreen();
    return new TimeScreen(Current);
};

ScreenModel *LanguageScreen::downModel() {
    if (menu_actions::hasConfig()) {
        return new BackScreen();
    } else {
        return new PairingScreen();
    }
};

ScreenModel *LanguageScreen::nextModel() {
    return new SetLanguageScreen(localization::currentLanguage());
};
