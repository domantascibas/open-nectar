#ifndef DEBUG_ERROR_SCREEN_H
#define DEBUG_ERROR_SCREEN_H

#include "../../menu_service.h"
#include "../ScreenModel.h"

struct DebugErrorScreen : ScreenModel {
  DebugErrorScreen() : ScreenModel() {
    refreshRate = 1;
    screenType = DebugError;
    
    title = "DebugErrorScreen";
    subtitle = itos(menu_actions::error());
    
    hasDownModel = true;
    hasUpModel = true;
  };
  
  virtual ScreenModel *upModel();
  virtual ScreenModel *downModel();
};

#endif
