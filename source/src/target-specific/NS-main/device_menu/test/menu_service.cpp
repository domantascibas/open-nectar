#include "../src/menu_service.h"
#include "../src/Screens/01/ActiveStatusScreen.h"
#include <iostream>

ScreenModel *m_screen = new ActiveStatusScreen();

void render() {

  std::cout << (m_screen->hasUpModel ? "⇧" : "") << std::endl;
  std::cout << m_screen->title;

  if (m_screen->screenType == Subtitled) {
    std::cout << std::endl << m_screen->subtitle;
  }

  std::cout << (m_screen->hasNextModel ? " ⇨" : "") << std::endl;
  std::cout << (m_screen->hasDownModel ? "⇩" : "") << std::endl;
}

void setScreenModel(ScreenModel *screen_model) {
  delete m_screen;
  m_screen = screen_model;
  render();
}

namespace menu_service {
void upButtonPressed() {
  if (!m_screen->hasUpModel)
    return;
  setScreenModel(m_screen->upModel());
};

void downButtonPressed() {
  if (!m_screen->hasDownModel)
    return;
  setScreenModel(m_screen->downModel());
};

void nextButtonPressed() {
  if (!m_screen->hasNextModel)
    return;
  setScreenModel(m_screen->nextModel());
};
} // namespace menu_service

namespace menu_actions {
bool hasConfig() { return false; };

/*
 */
HeaterMode _mode = None;
const HeaterMode &deviceMode() { return _mode; };
void setDeviceMode(const HeaterMode &mode) { _mode = mode; };

/*
 */
bool _is_pairing_on;
void startPairing(const bool &start) { _is_pairing_on = start; };
bool isPairingOn() { return _is_pairing_on; };

/*
 */
void reset() { exit(0); }

/*
 */
bool isInTestMode() { return true; };

/*
 */
bool isInOnboarding() { return false; };

/*
 */
int8_t _dayTemperature = 55;
int8_t _nightTemperature = 40;
int8_t _maxTemperature = 75;

int8_t temperature(const TemperatureType &type) {
  switch (type) {
  case TemperatureDay:
    return _dayTemperature;
  case TemperatureNight:
    return _nightTemperature;
  case TemperatureMax:
    return _maxTemperature;
  }
};

void setTemperature(const TemperatureType &type, const int8_t &t) {
  switch (type) {
  case TemperatureDay:
    _dayTemperature = t;
    break;
  case TemperatureNight:
    _nightTemperature = t;
    break;
  case TemperatureMax:
    _maxTemperature = t;
    break;
  }
};

void endOnboarding(){};

/*
 */
time_hm _time(0, 0);
time_hm _time_day_start(6, 0);
time_hm _time_night_start(23, 0);
time_hm getTime(const TimeType &type) {
  switch (type) {
  case Current:
    return _time;
  case DayStart:
    return _time_day_start;
  case NightStart:
    return _time_night_start;
  }
}
void setTime(const time_hm &time, const TimeType &type) {
  switch (type) {
  case Current:
    _time = time;
    break;
  case DayStart:
    _time_day_start = time;
    break;
  case NightStart:
    _time_night_start = time;
    break;
  }
}
} // namespace menu_actions