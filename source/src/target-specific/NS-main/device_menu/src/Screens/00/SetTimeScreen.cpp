#include "SetTimeScreen.h"

#include "../../menu_service.h"
#include "../01/ActiveStatusScreen.h"
#include "../02/TimeScreen.h"

ScreenModel *SetTimeScreen::upModel() {
    return new SetTimeScreen(type, isHours ? time++ : ++time, isHours);
};

ScreenModel *SetTimeScreen::downModel() {
    return new SetTimeScreen(type, isHours ? time-- : --time, isHours);
};

ScreenModel *SetTimeScreen::nextModel() {
    if (isHours)
        return new SetTimeScreen(type, time, false);
    if (!menu_actions::hasConfig()) {
        menu_actions::setTime(time, type);
    }
    if (menu_actions::isInOnboarding()) {
        menu_actions::endOnboarding();
        return new ActiveStatusScreen();
    } else {
        return new TimeScreen(type);
    }
};
