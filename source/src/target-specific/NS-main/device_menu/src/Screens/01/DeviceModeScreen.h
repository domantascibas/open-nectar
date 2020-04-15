#ifndef DEVICE_MODE_SCREEN_H
#define DEVICE_MODE_SCREEN_H

#include "../../menu_service.h"
#include "../ScreenModel.h"

struct DeviceModeScreen : ScreenModel {
  DeviceModeScreen() : ScreenModel() {

    hasUpModel = true;
    hasDownModel = true;
    hasNextModel = true;

    title = LocalizedString(LocalizationDeviceMode);
    switch (menu_actions::deviceMode()) {
    case None:
      subtitle = LocalizedString(LocalizationDefault);
      icon = ICON_AUTO;
      break;
    case Boost:
      subtitle = LocalizedString(LocalizationBoost);
      icon = ICON_BOOST;
      break;
    case Nogrid:
      subtitle = LocalizedString(LocalizationNoGrid);
      icon = ICON_NO_GRID;
      break;
    case Away:
      subtitle = LocalizedString(LocalizationAway);
      icon = ICON_AWAY;
      break;
		case Alloff:
			subtitle = "All Off";
      icon = ICON_AUTO;
			break;
    }
    screenType = Subtitled;
  };

  virtual ScreenModel *upModel();
  virtual ScreenModel *downModel();
  virtual ScreenModel *nextModel();
};

#endif
