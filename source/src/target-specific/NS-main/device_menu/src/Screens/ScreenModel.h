#ifndef _NS_SCREEN_MODEL_H
#define _NS_SCREEN_MODEL_H

#include "../Localizations/localization.h"
#include "Icons.h"
#include "stdint.h"
#include <string>

enum ScreenType { Titled, Subtitled, Selector, Idle, IdleError, ActiveStatus, ActiveTotal, Debug, DebugError };

struct ScreenSelector {
  uint8_t numberOfItems;
  uint8_t selected;
  ScreenSelector(uint8_t numberOfItems, uint8_t selected)
      : numberOfItems(numberOfItems), selected(selected){};
};

struct ScreenModel {
  ScreenModel()
      : hasUpModel(false), hasDownModel(false), hasNextModel(false),
        icon(ICON_NONE), screenType(Titled), selector(ScreenSelector(0, 0)),
        refreshRate(0), title(""), subtitle(""){};

  bool hasUpModel;
  bool hasDownModel;
  bool hasNextModel;
  ICON icon;

  ScreenSelector selector;
  ScreenType screenType;
  uint8_t refreshRate; // update screen after XX seconds

  std::string title;
  std::string subtitle;

  virtual ScreenModel *upModel() { return new ScreenModel(); }
  virtual ScreenModel *downModel() { return new ScreenModel(); }
  virtual ScreenModel *nextModel() { return new ScreenModel(); }
};

#endif
