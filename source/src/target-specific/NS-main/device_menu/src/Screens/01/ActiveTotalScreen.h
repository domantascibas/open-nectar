#ifndef ACTIVE_TOTAL_SCREEN_H
#define ACTIVE_TOTAL_SCREEN_H

#include "../ScreenModel.h"

struct ActiveTotalScreen : ScreenModel {
    ActiveTotalScreen() : ScreenModel() {
        refreshRate = 2;
        hasUpModel = true;
        hasDownModel = true;
        title = "ActiveTotalScreen";
        screenType = ActiveTotal;
    };

    virtual ScreenModel *upModel();
    virtual ScreenModel *downModel();
};

#endif
