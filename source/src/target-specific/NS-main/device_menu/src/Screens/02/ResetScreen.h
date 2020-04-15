#ifndef RESET_SCREEN_H
#define RESET_SCREEN_H

#include "../ScreenModel.h"

struct ResetScreen : ScreenModel {
    ResetScreen() : ScreenModel() {
        hasUpModel = true;
        hasDownModel = true;
        hasNextModel = true;

        title = LocalizedString(LocalizationReset);
        screenType = Titled;
        icon = ICON_RESET;
    };

    virtual ScreenModel *upModel();
    virtual ScreenModel *downModel();
    virtual ScreenModel *nextModel();
};

#endif
