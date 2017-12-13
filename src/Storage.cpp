#include "storage.h"
#include "eeprom.h"
#include "ErrorHandler.h"

// day temp, night temp, max temp
// day starts, night starts
// language
// current heating mode

uint16_t StoredItem::load() {
  EE_ReadVariable(lo_address, &storedItem);
  return storedItem;
}

void StoredItem::save(uint16_t item) {
  storedItem = item;
  EE_WriteVariable(lo_address, storedItem);
}

namespace Storage {
  StoredItem dayTemperature(0x4655);
  StoredItem nightTemperature(0x4755);
  StoredItem maxTemperature(0x4855);
  StoredItem dayStartsTime(0x4955);
  StoredItem nightStartsTime(0x4A55);
  StoredItem selectedLanguage(0x4B55);
  StoredItem selectedHeaterMode(0x4C55);
  
  static const uint8_t DEVICE_CONFIGURED = 0xC0;
  static const uint16_t CONFIG_ADDRESS = 0x4555;
  uint16_t config;
  
  void init() {
    if(FLASH->CR & (0x1 << 0x7)) {
      FLASH->KEYR = 0x45670123; //FLASH KEY 1
      FLASH->KEYR = 0xCDEF89AB; //FLASH KEY 2
      mainBoardError.clear_error(FLASH_ACCESS_ERROR);
    }
    
    EE_Init();
    EE_ReadVariable(CONFIG_ADDRESS, &config);
  }
  
  bool isConfigured() {
    if(config == DEVICE_CONFIGURED) {
      loadConfigData();
      return true;
    } else return false;
  }
  
  void saveConfig() {
    EE_WriteVariable(CONFIG_ADDRESS, DEVICE_CONFIGURED);
    
    saveTemp(TemperatureDay, menu_actions::temperature(TemperatureDay));
    saveTemp(TemperatureNight, menu_actions::temperature(TemperatureNight));
    saveTemp(TemperatureMax, menu_actions::temperature(TemperatureMax));
    
    saveTime(menu_actions::getTime(DayStart), DayStart);
    saveTime(menu_actions::getTime(NightStart), NightStart);
    
    saveLanguage(localization::currentLanguage());
    saveHeaterMode(DataService::getCurrentHeaterMode());
  }
  
  void clearConfig() {
    EE_WriteVariable(CONFIG_ADDRESS, 0xFF);
  }
  
  void loadConfigData() {
    menu_actions::setTemperature(TemperatureDay, dayTemperature.load());
    menu_actions::setTemperature(TemperatureNight, nightTemperature.load());
    menu_actions::setTemperature(TemperatureMax, maxTemperature.load());
    
    menu_actions::setTime(loadTime(DayStart), DayStart);
    menu_actions::setTime(loadTime(NightStart), NightStart);
    
    localization::setLanguage((const Language)selectedLanguage.load());
    DataService::setCurrentHeaterMode((nectar_contract::HeaterMode)selectedHeaterMode.load());
  }
  
  void saveTemp(const TemperatureType &type, const int8_t &t) {
    switch(type) {
      case TemperatureDay:
        dayTemperature.save((uint16_t)t);
        break;
      
      case TemperatureNight:
        nightTemperature.save((uint16_t)t);
        break;
      
      case TemperatureMax:
        maxTemperature.save((uint16_t)t);
        break;
    }
  }
  
  void saveTime(const time_hm &timeItem, const TimeType &type) {
    switch(type) {
      case DayStart:
        dayStartsTime.save(timeItem.hours*100 + timeItem.minutes);
        break;
      
      case NightStart:
        nightStartsTime.save(timeItem.hours*100 + timeItem.minutes);
        break;
      
      case Current:
        break;
    }
  }
  
  void saveLanguage(const Language &language) {
    selectedLanguage.save((uint16_t)language);
  }
  
  void saveHeaterMode(nectar_contract::HeaterMode mode) {
    selectedHeaterMode.save((uint16_t)mode);
  }
  
  time_hm loadTime(const TimeType &type) {
    uint8_t hrs, mins;
    uint16_t loadedTime;
    
    switch(type) {
      case DayStart:
        loadedTime = dayStartsTime.load();
        break;
      
      case NightStart:
        loadedTime = nightStartsTime.load();
        break;
      
      case Current:
        break;
    }
    
    mins = loadedTime % 100;
    hrs = (loadedTime - mins) / 100;
    time_hm new_time(hrs, mins);
    return new_time;
  }
};

// *******************************Nectar Sun Copyright ɠNectar Sun 2017*************************************   
