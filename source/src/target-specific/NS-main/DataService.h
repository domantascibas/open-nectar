#ifndef _NS_DATA_SERVICE_H
#define _NS_DATA_SERVICE_H

#include "NectarContract.h"
#include "DeviceMode.h"
#include "EspComms.h"

namespace DataService {
nectar_contract::HeaterMode getCurrentHeaterMode(void);
nectar_contract::HeaterMode getPreviousHeaterMode(void);

void resetData(void);

void updateHeaterMode(nectar_contract::HeaterMode, nectar_contract::HeaterMode);
void setCurrentHeaterMode(nectar_contract::HeaterMode);
void setPreviousHeaterMode(void);

bool isGridRelayOn(void);
bool isSunRelayOn(void);

void calculateSolarKwhDiff(bool);
float getSolarKwhToday(void);
float getBoilerPower(void);

void setCalibrate(bool);
bool getCalibrate(void);
bool isDayTime(void);
void modeSelected(void);
bool isModeSelected(void);
};

struct TemperatureData {
    TemperatureData() : dayTemperature(55.0), nightTemperature(40.0), minTemperature(5.0), maxTemperature(75.0), boilerTemperature(0.0), deviceTemperature(0.0) {};
    void setDayTemperature(float);
    void setNightTemperature(float);
    void setMaxTemperature(float);
    void setBoilerTemperature(float);
    void setDeviceTemperature(float);
    float getTemperature(void);
    float getDayTemperature(void);
    float getNightTemperature(void);
    float getMinTemperature(void);
    float getMaxTemperature(void);
    float getBoilerTemperature(void);
    float getDeviceTemperature(void);

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

void temperatureData_setBoilerTemperature(uint8_t temp);

#endif
