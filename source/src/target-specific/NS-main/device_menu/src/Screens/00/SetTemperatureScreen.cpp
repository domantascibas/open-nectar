#include "SetTemperatureScreen.h"

#include "../../menu_service.h"
#include "../02/TemperatureScreen.h"

ScreenModel *SetTemperatureScreen::upModel() {
  return new SetTemperatureScreen(type, temperature + 1);
};

ScreenModel *SetTemperatureScreen::downModel() {
  return new SetTemperatureScreen(type, temperature - 1);
};

ScreenModel *SetTemperatureScreen::nextModel() {
  menu_actions::setTemperature(type, temperature);
  return new TemperatureScreen(type);
};
