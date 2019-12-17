#include "consts.h"
#include "Storage.h"
#include "error_controller.h"

// day temp, night temp, max temp
// day starts, night starts
// language
// current heater mode, previous heater mode

namespace Storage {	
  // uint8_t config = EMPTY_VALUE;
  // uint8_t esp_config = EMPTY_VALUE;
  
  // bool isLanguageLoaded = false;
	// bool emptyStorage = true;
	// bool batchSaving = false;

	EE_SettingsDatastruct sDataStruct = {
		// config,
		// esp_config,
		// (uint8_t)DataService::getCurrentHeaterMode(),
		// (uint8_t)DataService::getPreviousHeaterMode(),
		// (menu_actions::getTime(DayStart).hours*100 + menu_actions::getTime(DayStart).minutes),
		// (menu_actions::getTime(NightStart).hours*100 + menu_actions::getTime(NightStart).minutes),
		// localization::currentLanguage(),
		// (uint16_t)menu_actions::temperature(TemperatureDay),
		// (uint16_t)menu_actions::temperature(TemperatureNight),
		// (uint16_t)menu_actions::temperature(TemperatureMax)
	};

  void init() {
    // __disable_irq();
    // if(FLASH->CR & (0x1 << 0x7)) {
    //   FLASH->KEYR = 0x45670123; //FLASH KEY 1
    //   FLASH->KEYR = 0xCDEF89AB; //FLASH KEY 2
    //   mainBoardError.clear_error(FLASH_ACCESS_ERROR);
    // }
    
    // EE_Init();
		// readData();
		// __enable_irq();
  }
	
	uint16_t saveData() {
		// if(!batchSaving) {
		// 	uint16_t pagestatus = EE_WriteDatastruct(&sDataStruct);
		// 	return pagestatus;
		// }
	}
	
	bool readData() {
		// bool emptyStorage = true;
		// EE_SettingsDatastruct rDataStruct;
		// emptyStorage = EE_ReadDatastruct(&rDataStruct);
		
		// if(emptyStorage) {
		// 	sDataStruct.device_config = EMPTY_VALUE;
		// 	sDataStruct.esp_config = EMPTY_VALUE;
		// } else {
		// 	sDataStruct = rDataStruct;
		// }
		
		// return emptyStorage;
	}
  
  bool isEspConfigured() {
    // if(sDataStruct.esp_config == ESP_CONFIGURED) return true;
    // else return false;
  }
  
  bool isConfigured() {
    // if(sDataStruct.device_config == DEVICE_CONFIGURED) return true;
    // else return false;
  }
  
  void saveEspConfig() {
    // __disable_irq();
    // sDataStruct.esp_config = ESP_CONFIGURED;
		// saveData();
    // __enable_irq();
  }
  
  void saveConfig() {
		// batchSaving = true;
		// saveTemp(TemperatureDay, menu_actions::temperature(TemperatureDay));
    // saveTemp(TemperatureNight, menu_actions::temperature(TemperatureNight));
    // saveTemp(TemperatureMax, menu_actions::temperature(TemperatureMax));
    
    // saveTime(menu_actions::getTime(DayStart), DayStart);
    // saveTime(menu_actions::getTime(NightStart), NightStart);
    
    // saveLanguage(localization::currentLanguage());
    // saveHeaterMode(DataService::getCurrentHeaterMode(), DataService::getPreviousHeaterMode());
		
		// batchSaving = false;
		// __disable_irq();
		// sDataStruct.device_config = DEVICE_CONFIGURED;
    // saveData();
    // __enable_irq();
  }
  
  void clearConfig() {
    // __disable_irq();
		// sDataStruct.device_config = EMPTY_VALUE;
		// sDataStruct.esp_config = EMPTY_VALUE;
		// saveData();
    // __enable_irq();
  }
  
  void loadConfigData() {
//     __disable_irq();
// //		readData();
		
//     menu_actions::setTemperature(TemperatureDay, sDataStruct.day_temperature);
//     menu_actions::setTemperature(TemperatureNight, sDataStruct.night_temperature);
//     menu_actions::setTemperature(TemperatureMax, sDataStruct.max_temperature);
    
//     menu_actions::setTime(loadTime(DayStart), DayStart);
//     menu_actions::setTime(loadTime(NightStart), NightStart);
    
//     localization::setLanguage((const Language)sDataStruct.language);
//     isLanguageLoaded = true;
//     DataService::updateHeaterMode((nectar_contract::HeaterMode)sDataStruct.selected_heater_mode, (nectar_contract::HeaterMode)sDataStruct.previous_heater_mode);
//     __enable_irq();
  }
  
  void saveTemp(const TemperatureType &type, const int8_t &t) {
    // __disable_irq();
    // switch(type) {
    //   case TemperatureDay:
		// 		sDataStruct.day_temperature = (uint16_t)t;
    //     break;
      
    //   case TemperatureNight:
    //     sDataStruct.night_temperature = (uint16_t)t;
    //     break;
      
    //   case TemperatureMax:
    //     sDataStruct.max_temperature = (uint16_t)t;
    //     break;
    // }
		// saveData();
    // __enable_irq();
  }
  
  // void saveTime(const time_hm &timeItem, const TimeType &type) {
  //   // __disable_irq();
  //   // switch(type) {
  //   //   case DayStart:
	// 	// 		sDataStruct.day_starts_time = timeItem.hours*100 + timeItem.minutes;
  //   //     break;
      
  //   //   case NightStart:
	// 	// 		sDataStruct.night_starts_time = timeItem.hours*100 + timeItem.minutes;
  //   //     break;
      
  //   //   case Current:
  //   //     break;
  //   // }
	// 	// saveData();
  //   // __enable_irq();
  // }
  
  void saveLanguage(const Language &language) {
    // __disable_irq();
    // if(isConfigured()) {
    //   printf("LANGUAGE configured\n");
    //   if(isLanguageLoaded) {
    //     printf("LANGUAGE SET %d\n", language);
		// 		sDataStruct.language = (uint16_t)language;
    //   }
    // } else {
    //   printf("LANGUAGE SET %d\n", language);
		// 	sDataStruct.language = (uint16_t)language;
    // }
		// saveData();
    // __enable_irq();
  }
  
  void saveHeaterMode(nectar_contract::HeaterMode currMode, nectar_contract::HeaterMode prevMode) {
    // __disable_irq();
		// sDataStruct.selected_heater_mode = (uint16_t)currMode;
		// sDataStruct.previous_heater_mode = (uint16_t)prevMode;
		// saveData();
    // __enable_irq();
  }
  
  void saveCurrentHeaterMode(nectar_contract::HeaterMode mode) {
    // __disable_irq();
    // sDataStruct.selected_heater_mode = (uint16_t)mode;
		// saveData();
    // __enable_irq();
  }
  
  void savePreviousHeaterMode(nectar_contract::HeaterMode mode) {
    // __disable_irq();
    // sDataStruct.previous_heater_mode = (uint16_t)mode;
		// saveData();
    // __enable_irq();
  }
  
  // time_hm loadTime(const TimeType &type) {
    // uint8_t hrs, mins;
    // uint16_t loadedTime;
    
    // switch(type) {
    //   case DayStart:
    //     loadedTime = sDataStruct.day_starts_time;
    //     break;
      
    //   case NightStart:
    //     loadedTime = sDataStruct.night_starts_time;
    //     break;
      
    //   case Current:
    //     break;
    // }
    
    // mins = loadedTime % 100;
    // hrs = (loadedTime - mins) / 100;
    // time_hm new_time(hrs, mins);
    // return new_time;
  // }
};
