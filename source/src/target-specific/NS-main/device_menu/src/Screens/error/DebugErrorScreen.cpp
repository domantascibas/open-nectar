#include "DebugErrorScreen.h"

#include "../engineering_menu/CalibrateScreen.h"
#include "DebugScreen.h"

ScreenModel *DebugErrorScreen::upModel() {
    return new DebugScreen();
};

ScreenModel *DebugErrorScreen::downModel() {
    return new CalibrateScreen();
};
