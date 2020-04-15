#ifndef ACTIVE_STATUS_SCREEN_H
#define ACTIVE_STATUS_SCREEN_H

#include "../ScreenModel.h"

struct ActiveStatusScreen : ScreenModel {
    ActiveStatusScreen() : ScreenModel() {
        refreshRate = 2;
        hasDownModel = true;
        hasUpModel = true;
        title = "ActiveStatusScreen";
        screenType = ActiveStatus;
    };

    virtual ScreenModel *downModel();
    virtual ScreenModel *upModel();
};

#endif
