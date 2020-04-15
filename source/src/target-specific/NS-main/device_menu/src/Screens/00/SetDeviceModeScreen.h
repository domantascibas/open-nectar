#ifndef SET_DEVICE_MODE_SCREEN_H
#define SET_DEVICE_MODE_SCREEN_H

#include "../../helpers/helpers.h"
#include "../../menu_service.h"
#include "../ScreenModel.h"

struct SetDeviceModeScreen : ScreenModel {
    SetDeviceModeScreen(const HeaterMode &mode) : ScreenModel(), mode(mode) {
        hasUpModel = true;
        hasDownModel = true;
        hasNextModel = true;

        switch (mode) {
            case None:
                title = LocalizedString(LocalizationDefault);
                icon = ICON_AUTO;
                break;
            case Boost:
                title = LocalizedString(LocalizationBoost);
                icon = ICON_BOOST;
                break;
            case Nogrid:
                title = LocalizedString(LocalizationNoGrid);
                icon = ICON_NO_GRID;
                break;
            case Away:
                title = LocalizedString(LocalizationAway);
                icon = ICON_AWAY;
                break;
            case Alloff:
                title = "All off";
                icon = ICON_AUTO;
        }

        selector = ScreenSelector(1, 0);
        screenType = Selector;
//    screenType = Titled;
    };

    HeaterMode mode;

    virtual ScreenModel *upModel();
    virtual ScreenModel *downModel();
    virtual ScreenModel *nextModel();
};

#endif
