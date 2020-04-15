#ifndef SET_LANGUAGE_SCREEN_H
#define SET_LANGUAGE_SCREEN_H

#include "../ScreenModel.h"

struct SetLanguageScreen : ScreenModel {
  SetLanguageScreen(const Language &language)
      : ScreenModel(), language(language) {
    if(LanguageCout > 1) {
      hasUpModel = true;
      hasDownModel = true;
    } else {
      hasUpModel = false;
      hasDownModel = false;
    }
    hasNextModel = true;

    title = localization::languageName(language);
    selector = ScreenSelector(1, 0);
    screenType = Selector;
  };

  Language language;

  virtual ScreenModel *upModel();
  virtual ScreenModel *downModel();
  virtual ScreenModel *nextModel();
};

#endif
