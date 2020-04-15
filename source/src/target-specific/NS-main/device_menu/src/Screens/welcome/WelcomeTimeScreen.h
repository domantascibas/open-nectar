#ifndef WELCOME_SET_TIME_SCREEN_H
#define WELCOME_SET_TIME_SCREEN_H

#include "../../menu_service.h"
#include "../ScreenModel.h"

struct WelcomeTimeScreen : ScreenModel {
  WelcomeTimeScreen(TimeType type)
      : ScreenModel(), type(type), time(menu_actions::getTime(type)) {

    hasNextModel = true;
    title = LocalizedString(LocalizationSetTime);
    icon = ICON_TIME;

    screenType = Titled;
  };

  TimeType type;
  time_hm time;

  virtual ScreenModel *nextModel();
};

#endif
