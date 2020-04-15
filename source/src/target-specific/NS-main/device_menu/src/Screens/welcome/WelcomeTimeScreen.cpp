#include "WelcomeTimeScreen.h"

#include "../../menu_service.h"
#include "../00/SetTimeScreen.h"

ScreenModel *WelcomeTimeScreen::nextModel() {
    return new SetTimeScreen(type, time, true, true);
};
