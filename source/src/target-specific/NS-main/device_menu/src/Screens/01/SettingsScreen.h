#ifndef SETTINGS_SCREEN_H
#define SETTINGS_SCREEN_H

#include "../ScreenModel.h"

struct SettingsScreen : ScreenModel {
  SettingsScreen() : ScreenModel() {
    hasUpModel = true;
    hasDownModel = true;
    hasNextModel = true;
    title = LocalizedString(LocalizationSettings);
    icon = ICON_SETTINGS;
  };

  virtual ScreenModel *upModel();
  virtual ScreenModel *downModel();
  virtual ScreenModel *nextModel();
};

#endif
