#ifndef PAIRING_SCREEN_H
#define PAIRING_SCREEN_H

#include "../../menu_service.h"
#include "../ScreenModel.h"

struct PairingScreen : ScreenModel {
  PairingScreen() : ScreenModel() {
    hasUpModel = true;
    hasDownModel = true;
    hasNextModel = true;
    
    refreshRate = 5;

    title = LocalizedString(LocalizationPairing);
    subtitle = menu_actions::getPin();
    screenType = Subtitled;
    icon = menu_actions::isPairingOn() ? ICON_PAIR_ACTIVE : ICON_PAIR_INACTIVE;
  };

  virtual ScreenModel *upModel();
  virtual ScreenModel *downModel();
  virtual ScreenModel *nextModel();
};

#endif
