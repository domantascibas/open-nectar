#ifndef BACK_SCREEN_H
#define BACK_SCREEN_H

#include "../ScreenModel.h"

struct BackScreen : ScreenModel {
    BackScreen() : ScreenModel() {
        hasUpModel = true;
        hasDownModel = true;
        hasNextModel = true;

        title = LocalizedString(LocalizationBack);
        screenType = Titled;
        icon = ICON_BACK;
    };

    virtual ScreenModel *upModel();
    virtual ScreenModel *downModel();
    virtual ScreenModel *nextModel();
};

#endif
