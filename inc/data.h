#ifndef DATA_H
#define DATA_H

//Constants
#define DRIVER_ON                   0
#define DRIVER_OFF                  1
#define ACK                         0x06
#define NACK                        0x15
#define INCOMING_DATA               0x25    //'%'
#define WAITING_FOR_DATA            0x26    //'&'
#define BAD_CMD                     0x21    //'!'
#define HANDSHAKE                   0x5E

enum error_codes{
  SETUP_ERROR = 0x01,
  STARTUP_ERROR,
  ADC_ERROR,                  //can't find both ADC sensors
  ADC_VOLTAGE_ERROR,
  ADC_CURRENT_ERROR,
  ADC_SETUP_ERROR,
  FLASH_ACCESS_ERROR,
  FLASH_READ_ERROR,
  FLASH_WRITE_ERROR,
  CALIBRATION_ERROR,          //no calibration data
  DC_OVER_VOLTAGE,            //V_pv > 350V
  DC_OVER_CURRENT,            //I_pv > 10A
  DC_CURRENT_LEAKS,           //could be a faulty relay, or a short
  I2C_ERROR,
  PWM_OK,
  PWM_ERROR,
  BOARD_CONFIG_ERROR,
  OVERHEAT,
  RADIATOR_OVERHEAT,
  AIRGAP_OVERHEAT,
  RESTART_REQUIRED = 0xAA,
  BOILER_TEMP_SENSOR_ERROR
};

enum codes {
  NS_OK,
  NS_ERROR,
  MSG_ERROR
};

struct Data {
  uint8_t current_mode;

  float temp_max;
  float temp_min;
  float temp_scheduled;
  float temp_boiler;

  bool  sun_relay_on;
  bool  grid_relay_on;

  float pv_power;
  float pv_voltage;
  float pv_current;
  float pv_ref_voltage;
  float pv_ref_current;
  bool  pv_available;

  float pwm_duty;
  float airgap_temp;
  float radiator_temp;
  float device_temperature;

  uint8_t mosfet_overheat_on;
  uint8_t error;
};

extern Data data;

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************
