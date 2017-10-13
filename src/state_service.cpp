#include "state_service.h"
#include "NectarContract.h"

namespace StateService {
  DeviceState currentDeviceState = WELCOME;
  
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
  
  DeviceState *get_state() {
    return &currentDeviceState;
  }
  
  void set_state(DeviceState state) {
    currentDeviceState = state;
  }
}
