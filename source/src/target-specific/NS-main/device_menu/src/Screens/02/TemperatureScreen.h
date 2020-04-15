#ifndef TEMPERATURE_SCREEN_H
#define TEMPERATURE_SCREEN_H

#include "../../helpers/helpers.h"
#include "../../menu_service.h"
#include "../ScreenModel.h"

struct TemperatureScreen : ScreenModel {
  TemperatureScreen(const TemperatureType &type)
      : ScreenModel(), type(type),
        temperature(menu_actions::temperature(type)) {

    hasUpModel = true;
    hasDownModel = true;
    hasNextModel = true;

    switch (type) {
    case TemperatureDay:
      title = LocalizedString(LocalizationDayTemp);
      break;
    case TemperatureNight:
      title = LocalizedString(LocalizationNightTemp);
      break;
    case TemperatureMax:
      title = LocalizedString(LocalizationMaxTemp);
      break;
    }

    subtitle = itos(temperature) + "°";
    screenType = Subtitled;
    icon = ICON_TEMPERATURE;
  };

  int8_t temperature;
  TemperatureType type;

  virtual ScreenModel *upModel();
  virtual ScreenModel *downModel();
  virtual ScreenModel *nextModel();
};

#endif
