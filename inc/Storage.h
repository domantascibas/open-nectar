#ifndef STORAGE_H
#define STORAGE_H
#include <stdint.h>
#include "menu_service.h"
#include "DataService.h"
#include "localization.h"
#include "eeprom.h"

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
  void saveTime(const time_hm &, const TimeType &);
  void saveLanguage(const Language &);
  void saveHeaterMode(nectar_contract::HeaterMode, nectar_contract::HeaterMode);
  void saveCurrentHeaterMode(nectar_contract::HeaterMode);
  void savePreviousHeaterMode(nectar_contract::HeaterMode);
  
  time_hm loadTime(const TimeType &);
};

#endif

// *******************************Nectar Sun Copyright ɠNectar Sun 2017*************************************   
