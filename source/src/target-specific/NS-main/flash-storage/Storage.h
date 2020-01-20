#ifndef _NS_STORAGE_H
#define _NS_STORAGE_H
#include <stdint.h>
#include "menu_service.h"
#include "DataService.h"
#include "localization.h"
extern "C" {
#include "eeprom.h"
}

// typedef enum {
//     Current,
//     DayStart,
//     NightStart
// } TimeType;
// typedef enum {
//     TemperatureDay,
//     TemperatureNight,
//     TemperatureMax
// } TemperatureType;

// typedef enum {
//     EN,
//     DE,
//     RU,
//     LT,
//     LanguageCout
// } Language;

// typedef enum {
//     LocalizationSettings,
//     LocalizationReset,
//     LocalizationPairing,
//     LocalizationPairingActive,
//     LocalizationPairingInactive,
//     LocalizationPairingStart,
//     LocalizationLanguage,
//     LocalizationBack,
//     LocalizationDeviceMode,
//     LocalizationDefault,
//     LocalizationBoost,
//     LocalizationNoGrid,
//     LocalizationAway,
//     LocalizationTemperature,
//     LocalizationTime,
//     LocalizationDayStart,
//     LocalizationNightStart,
//     LocalizationConnected,
//     LocalizationNotConnected,
//     LocalizationToday,
//     LocalizationNightTemp,
//     LocalizationDayTemp,
//     LocalizationMaxTemp,
//     LocalizationYes,
//     LocalizationNo,
//     LocalizationEnable,
//     LocalizationEnabled,
//     LocalizationDisable,
//     LocalizationDisabled,
//     LocalizationSetTime,
//     LocalizationWelcome,
//     LocalizationSun,
//     LocalizationGrid,
//     LocalizationIdle,
//     LocalizationTotal,
// } LocalizationKey;

void storage_init(void);
uint8_t storage_isEspConfigured(void);
uint8_t storage_isConfigured(void);
void storage_saveEspConfig(void);
void storage_saveConfig(void);
void storage_clearConfig(void);
void storage_loadConfigData(void);

uint16_t storage_saveData(void);
uint8_t storage_readData(void);

namespace Storage {
void saveTemp(const TemperatureType &, const int8_t &);
void saveTime(const time_hm &, const TimeType &);
void saveLanguage(const Language &);
void saveHeaterMode(nectar_contract::HeaterMode, nectar_contract::HeaterMode);
void saveCurrentHeaterMode(nectar_contract::HeaterMode);
void savePreviousHeaterMode(nectar_contract::HeaterMode);

time_hm loadTime(const TimeType &);
};

#endif
