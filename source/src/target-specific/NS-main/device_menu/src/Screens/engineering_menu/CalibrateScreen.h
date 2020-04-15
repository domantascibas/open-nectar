#ifndef CALIBRATE_SCREEN_H
#define CALIBRATE_SCREEN_H

#include "../ScreenModel.h"

struct CalibrateScreen : ScreenModel {
    CalibrateScreen() : ScreenModel() {
        hasUpModel = true;
        hasDownModel = true;
        hasNextModel = true;

        screenType = Titled;
        icon = ICON_RESET;
        title = "Calibrate";
    };

    virtual ScreenModel *upModel();
    virtual ScreenModel *nextModel();
    virtual ScreenModel *downModel();
};

#endif
