#include "state_service.h"
#include "NectarContract.h"

namespace StateService {
  typedef enum {
    WELCOME,
    NOT_CONFIGURED,
    CONFIGURED,
    TEST_MODE
  } DeviceState;
  
  DeviceState currentDeviceState, lastDeviceState;
  
  void setup() {
    //select device state to start with
    currentDeviceState = NOT_CONFIGURED;
  }
  
  void loop() {
    if(currentDeviceState != lastDeviceState) {
      lastDeviceState = currentDeviceState;
      switch(currentDeviceState) {
        default:
        case NOT_CONFIGURED:
          printf("gData %d %f %f\r\n", *get_device_mode(), get_temperature(), get_temperature_max());
          break;
        
        case CONFIGURED:
          //DeviceStateData = pointer to ESP data struct
  //        printf("gData %d %f %f\r\n", gData->current_mode, gData->temperature, gData->temperature_max);
          break;
        
        case WELCOME:
          break;
        
        case TEST_MODE:
          break;
      }
    }
  }
  
  float get_temperature() {
    if(currentDeviceState == CONFIGURED) {
      return EspDeviceData.temperature;
    } else {
      return data.temp_scheduled;
    }
  }
  
  float get_temperature_max() {
    if(currentDeviceState == CONFIGURED) {
      return EspDeviceData.temperature_max;
    } else {
      return data.temp_max;
    }
  }
  
  float get_boiler_power() {
    if(currentDeviceState == CONFIGURED) {
      return EspDeviceData.boilerPower;
    } else {
      return data.boiler_power;
    }
  }
  
  HeaterMode *get_device_mode() {
    if(currentDeviceState == CONFIGURED) {
      return (HeaterMode *)&EspDeviceData.heater_mode;
    } else {
      return &data.current_mode;
    }
  }
}
