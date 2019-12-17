#ifndef _NS_STORAGE_H
#define _NS_STORAGE_H
#include <stdint.h>
// #include "menu_service.h"
#include "DataService.h"
// #include "localization.h"
#include "eeprom.h"

enum TimeType { Current, DayStart, NightStart };
enum TemperatureType { TemperatureDay, TemperatureNight, TemperatureMax };

enum Language { EN, DE, RU, LT, LanguageCout };

enum LocalizationKey {
    LocalizationSettings,
    LocalizationReset,
    LocalizationPairing,
    LocalizationPairingActive,
    LocalizationPairingInactive,
    LocalizationPairingStart,
    LocalizationLanguage,
    LocalizationBack,
    LocalizationDeviceMode,
    LocalizationDefault,
    LocalizationBoost,
    LocalizationNoGrid,
    LocalizationAway,
    LocalizationTemperature,
    LocalizationTime,
    LocalizationDayStart,
    LocalizationNightStart,
    LocalizationConnected,
    LocalizationNotConnected,
    LocalizationToday,
    LocalizationNightTemp,
    LocalizationDayTemp,
    LocalizationMaxTemp,
    LocalizationYes,
    LocalizationNo,
    LocalizationEnable,
    LocalizationEnabled,
    LocalizationDisable,
    LocalizationDisabled,
    LocalizationSetTime,
    LocalizationWelcome,
    LocalizationSun,
    LocalizationGrid,
    LocalizationIdle,
    LocalizationTotal,
};

namespace Storage {
void init();
bool isEspConfigured();
bool isConfigured();
void saveEspConfig();
void saveConfig();
void clearConfig();
void loadConfigData();

uint16_t saveData();
bool readData();

void saveTemp(const TemperatureType &, const int8_t &);
// void saveTime(const time_hm &, const TimeType &);
void saveLanguage(const Language &);
void saveHeaterMode(nectar_contract::HeaterMode, nectar_contract::HeaterMode);
void saveCurrentHeaterMode(nectar_contract::HeaterMode);
void savePreviousHeaterMode(nectar_contract::HeaterMode);

// time_hm loadTime(const TimeType &);
};

#endif
