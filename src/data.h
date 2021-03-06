#ifndef _NS_DATA_H
#define _NS_DATA_H

#include "NectarContract.h"

#define LED_ON            0
#define LED_OFF           1

enum codes {
  NS_OK,
  NS_ERROR,
  MSG_OK,
  MSG_ERROR
};

enum modes {
  IDLE,
  RUNNING,
  STOP,
  MANUAL
};

struct Data {
  bool isCalibrating;
  bool isInOnboarding;
  bool isTestMode;
  uint8_t current_state;
  
  float moment_power;
  float moment_voltage;
  float moment_current;

  float reference_voltage;
  float reference_current;
  
  float grid_energy_meter_kwh;
  float sun_energy_meter_kwh;

  float pwm_duty;
  float device_temperature;

  bool mosfet_overheat_on;
  bool calibrated;
  bool generator_on;
  
  uint32_t error;
  bool readingSerial;
  bool readingTemperature;
  bool safeToReadTemp;
	bool startCalibration;
};

struct StorageData {
  float ref_voltage;
  float ref_current;
  float sun_meter;
  float grid_meter;
};

extern Data data;
extern StorageData storage_data;
extern nectar_contract::PowerBoardState powerState;

#endif
