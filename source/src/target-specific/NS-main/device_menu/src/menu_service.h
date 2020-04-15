#ifndef _NS_MENU_SERVICE_H
#define _NS_MENU_SERVICE_H

#include "Screens/ScreenModel.h"
#include "helpers/helpers.h"

enum TimeType { Current, DayStart, NightStart };
enum TemperatureType { TemperatureDay, TemperatureNight, TemperatureMax };

enum HeaterMode {
    None = 0,
    Boost = 1,
    Away = 2,
    Nogrid = 3,
    Alloff = 4,
    Count = 5
};

namespace menu_service {
extern volatile bool needUpdate;
extern bool resetScreen;
void enterTestScreen(void);
void updateScreen(void);
void setup(void);
void upButtonPressed(void);
void downButtonPressed(void);
void nextButtonPressed(void);
} // namespace menu_service

namespace menu_actions {
bool hasWifi(void);
bool hasConfig(void);

const HeaterMode &deviceMode(void);
void setDeviceMode(const HeaterMode &mode);

void reset(void);

void startPairing(const bool &start);
void calibratePowerBoard(void);
bool isPairingOn(void);
bool isInTestMode(void);
bool isInOnboarding(void);
bool isLoading(void);
std::string getPin(void);
void endOnboarding(void);
void modeSelected(void);

uint32_t error(void);
bool hasErrors(void);
bool isErrorCritical(uint32_t);

int8_t temperature(const TemperatureType &type);
void setTemperature(const TemperatureType &type, const int8_t &t);

void updateTime(void);
time_hm getTime(const TimeType &type);
void setTime(const time_hm &time, const TimeType &type);
} // namespace menu_actions

#endif
