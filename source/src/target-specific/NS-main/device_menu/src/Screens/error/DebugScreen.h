#ifndef DEBUG_SCREEN_H
#define DEBUG_SCREEN_H

#include "../ScreenModel.h"

struct DebugScreen : ScreenModel {
    DebugScreen() : ScreenModel() {
        refreshRate = 1;
        hasUpModel = true;
        hasDownModel = true;
        title = "DebugScreen";
        icon = ICON_NONE;
        screenType = Debug;
    };

    virtual ScreenModel *upModel();
    virtual ScreenModel *downModel();
};

#endif
