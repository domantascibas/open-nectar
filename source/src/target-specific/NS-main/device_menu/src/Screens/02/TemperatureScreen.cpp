#include "TemperatureScreen.h"

#include "../../menu_service.h"
#include "../00/SetTemperatureScreen.h"
#include "ResetScreen.h"
#include "TimeScreen.h"

ScreenModel *TemperatureScreen::upModel() {
  switch (type) {
  case TemperatureDay:
    return new ResetScreen();
  case TemperatureNight:
    return new TemperatureScreen(TemperatureDay);
  case TemperatureMax:
    return new TemperatureScreen(TemperatureNight);
  }
};

ScreenModel *TemperatureScreen::downModel() {
  switch (type) {
  case TemperatureDay:
    return new TemperatureScreen(TemperatureNight);
  case TemperatureNight:
    return new TemperatureScreen(TemperatureMax);
  case TemperatureMax:
    return new TimeScreen(DayStart);
  }
};

ScreenModel *TemperatureScreen::nextModel() {
  return new SetTemperatureScreen(type, temperature);
};
