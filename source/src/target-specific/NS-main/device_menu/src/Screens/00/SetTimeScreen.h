#ifndef SET_TIME_SCREEN_H
#define SET_TIME_SCREEN_H

#include "../ScreenModel.h"
#include "../../menu_service.h"

struct SetTimeScreen : ScreenModel
{
  SetTimeScreen(TimeType type, time_hm time, bool isHours = true, bool onboarding = false)
      : ScreenModel(), type(type), time(time), isHours(isHours)
  {

    hasUpModel = true;
    hasDownModel = true;
    hasNextModel = true;

    title = time.string();
    screenType = Selector;
    selector = ScreenSelector(2, isHours ? 0 : 1);
  };

  TimeType type;
  time_hm time;
  bool isHours;
  bool onboarding;

  virtual ScreenModel *upModel();
  virtual ScreenModel *downModel();
  virtual ScreenModel *nextModel();
};

#endif
