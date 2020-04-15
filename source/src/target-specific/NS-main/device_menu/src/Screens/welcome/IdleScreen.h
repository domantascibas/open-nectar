#ifndef IDLE_SCREEN_H
#define IDLE_SCREEN_H

#include "../ScreenModel.h"

struct IdleScreen : ScreenModel {
  IdleScreen() : ScreenModel() {
    refreshRate = 2;
    hasUpModel = true;
    hasDownModel = true;
    hasNextModel = true;
    title = "IdleScreen";
    screenType = Idle;
  };

  virtual ScreenModel *upModel();
  virtual ScreenModel *downModel();
  virtual ScreenModel *nextModel();
};

#endif
