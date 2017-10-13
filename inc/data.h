#ifndef DATA_H
#define DATA_H

#define LED_ON            0
#define LED_OFF           1

#define DRIVER_ON         0
#define DRIVER_OFF        1

const float VOLTAGE_LIMIT = 400.0;
const float CURRENT_LIMIT = 10.0;

//enum error_codes{
//  SETUP_ERROR = 0x01,
//  STARTUP_ERROR,
//  ADC_ERROR,
//  ADC_VOLTAGE_ERROR,
//  ADC_CURRENT_ERROR,
//  ADC_SETUP_ERROR,
//  FLASH_ACCESS_ERROR,
//  FLASH_READ_ERROR,
//  FLASH_WRITE_ERROR,
//  NOT_CALIBRATED,
//  DC_OVER_VOLTAGE,
//  DC_OVER_CURRENT,
//  DC_CURRENT_LEAKS,
//  I2C_ERROR,
//  PWM_OK,
//  PWM_ERROR,
//  BOARD_CONFIG_ERROR,
//  OVERHEAT,
//  RADIATOR_OVERHEAT,
//  AIRGAP_OVERHEAT,
//  NO_LOAD
//};

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
  SERVICE
};

struct Data {
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
  float airgap_temperature;
  float radiator_temperature;

  bool mosfet_overheat_on;
  bool calibrated;
  bool generator_on;
  
  uint8_t error;
};

struct StorageData {
  float ref_voltage;
  float ref_current;
  float sun_meter;
  float grid_meter;
};

extern Data data;
extern StorageData storage_data;

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
