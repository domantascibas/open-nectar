#ifndef DATA_SERVICE_H
#define DATA_SERVICE_H

#include "NectarContract.h"
#include "DeviceMode.h"
#include "EspComms.h"

namespace DataService {
  nectar_contract::HeaterMode getCurrentHeaterMode();
  nectar_contract::HeaterMode getPreviousHeaterMode();
  
  void resetData();
  
  void updateHeaterMode(nectar_contract::HeaterMode, nectar_contract::HeaterMode);
  void setCurrentHeaterMode(nectar_contract::HeaterMode);
  void setPreviousHeaterMode();
  
  bool isGridRelayOn();
  bool isSunRelayOn();
  
  void calculateSolarKwhDiff(bool);
  float getSolarKwhToday();
  float getBoilerPower();
	
	void setCalibrate(bool);
	bool getCalibrate(void);
	bool isDayTime(void);
};

struct TemperatureData {
  TemperatureData() : dayTemperature(55.0), nightTemperature(40.0), minTemperature(5.0), maxTemperature(75.0), boilerTemperature(0.0), deviceTemperature(0.0) {};
  void setDayTemperature(float);
  void setNightTemperature(float);
  void setMaxTemperature(float);
  void setBoilerTemperature(float);
  void setDeviceTemperature(float);
  float getTemperature();
  float getDayTemperature();
  float getNightTemperature();
  float getMinTemperature();
  float getMaxTemperature();
  float getBoilerTemperature();
  float getDeviceTemperature();
  
private:
  float dayTemperature;
  float nightTemperature;
  float minTemperature;
  float maxTemperature;
  float boilerTemperature;
  float deviceTemperature;
};

extern TemperatureData temperatureData;
extern nectar_contract::ESPState espData;
extern nectar_contract::PowerBoardState powerData;

#endif
