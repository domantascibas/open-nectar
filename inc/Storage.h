#ifndef STORAGE_H
#define STORAGE_H
#include <stdint.h>
#include "menu_service.h"
#include "DataService.h"
#include "localization.h"

//data stored in memory address 0x0802E000

struct StoredItem {
  StoredItem(const uint16_t address) 
    : lo_address(address), hi_address(address + 0x100) {};
  void save(uint16_t);
  uint16_t load();
  
private:
  const uint16_t lo_address;
  const uint16_t hi_address;
  
  uint16_t storedItem;
};

namespace Storage {
  void init();
  bool isEspConfigured();
  bool isConfigured();
  void saveEspConfig();
  void saveConfig();
  void clearConfig();
  void loadConfigData();
  
  void saveTemp(const TemperatureType &, const int8_t &);
  void saveTime(const time_hm &, const TimeType &);
  void saveLanguage(const Language &);
  void saveHeaterMode(nectar_contract::HeaterMode, nectar_contract::HeaterMode);
  void saveCurrentHeaterMode(nectar_contract::HeaterMode);
  void savePreviousHeaterMode(nectar_contract::HeaterMode);
  
  time_hm loadTime(const TimeType &);
};

#endif

// *******************************Nectar Sun Copyright ɠNectar Sun 2017*************************************   
