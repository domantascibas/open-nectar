#ifndef ERROR_SCREEN_H
#define ERROR_SCREEN_H

#include "../../menu_service.h"
#include "../ScreenModel.h"

struct ErrorScreen : ScreenModel {
    ErrorScreen() : ScreenModel() {
        refreshRate = 1;

        screenType = IdleError;
        title = "ERROR";
        subtitle = itos(menu_actions::error());

        if (menu_actions::isInTestMode()) {
            hasDownModel = true;
            hasUpModel = true;
        } else {
            hasDownModel = false;
            hasUpModel = false;
        }
    };

    virtual ScreenModel *upModel();
    virtual ScreenModel *downModel();
};

#endif
