#ifndef TIME_SCREEN_H
#define TIME_SCREEN_H

#include "../../menu_service.h"
#include "../ScreenModel.h"

struct TimeScreen : ScreenModel {
    TimeScreen(TimeType type)
        : ScreenModel(), type(type), time(menu_actions::getTime(type)) {

        hasUpModel = true;
        hasDownModel = true;
        hasNextModel = true;

        switch (type) {
            case Current:
                title = LocalizedString(LocalizationTime);
                icon = ICON_TIME;
                break;
            case DayStart:
                title = LocalizedString(LocalizationDayStart);
                icon = ICON_DAY;
                break;
            case NightStart:
                title = LocalizedString(LocalizationNightStart);
                icon = ICON_NIGHT;
                break;
        }

        refreshRate = 60;
        subtitle = time.string();
        screenType = Subtitled;
    };

    TimeType type;
    time_hm time;

    virtual ScreenModel *upModel();
    virtual ScreenModel *downModel();
    virtual ScreenModel *nextModel();
};

#endif
