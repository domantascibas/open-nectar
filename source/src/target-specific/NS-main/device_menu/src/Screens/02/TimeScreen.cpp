#include "TimeScreen.h"

#include "../../menu_service.h"
#include "../00/SetTimeScreen.h"
#include "LanguageScreen.h"
#include "TemperatureScreen.h"

ScreenModel *TimeScreen::upModel() {
  switch (type) {
  case DayStart:
    return new TemperatureScreen(TemperatureMax);
  case NightStart:
    return new TimeScreen(DayStart);
  case Current:
    return new TimeScreen(NightStart);
  }
};

ScreenModel *TimeScreen::downModel() {
  switch (type) {
  case DayStart:
    return new TimeScreen(NightStart);
  case NightStart:
    return new TimeScreen(Current);
  case Current:
    return new LanguageScreen();
  }
};

ScreenModel *TimeScreen::nextModel() {
  return new SetTimeScreen(type, time, true);
};
