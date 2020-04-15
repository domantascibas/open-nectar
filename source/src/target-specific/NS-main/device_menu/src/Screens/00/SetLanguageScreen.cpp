#include "SetLanguageScreen.h"
#include "../../menu_service.h"
#include "../02/LanguageScreen.h"
#include "../welcome/WelcomeTimeScreen.h"

using namespace localization;

ScreenModel *SetLanguageScreen::upModel() {
    return new SetLanguageScreen(prevLanguage(language));
};

ScreenModel *SetLanguageScreen::downModel() {
    return new SetLanguageScreen(nextLanguage(language));
};

ScreenModel *SetLanguageScreen::nextModel() {
    setLanguage(language);
    saveLanguage();
    if (menu_actions::isInOnboarding()) return new WelcomeTimeScreen(Current);
    else return new LanguageScreen();
};
