#ifndef LANGUAGE_SCREEN_H
#define LANGUAGE_SCREEN_H

#include "../ScreenModel.h"

struct LanguageScreen : ScreenModel {
  LanguageScreen() : ScreenModel() {
    hasUpModel = true;
    hasDownModel = true;
    hasNextModel = true;

    title = LocalizedString(LocalizationLanguage);
    subtitle = localization::languageName(localization::currentLanguage());
    screenType = Subtitled;
    icon = ICON_LANGUAGE;
  };

  virtual ScreenModel *upModel();
  virtual ScreenModel *downModel();
  virtual ScreenModel *nextModel();
};

#endif
