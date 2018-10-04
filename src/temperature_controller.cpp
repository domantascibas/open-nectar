#include "consts.h"
#include "temperature_controller.h"
#include "processor_temperature.h"
#include "temperature_sensor.h"
#include "error_controller.h"
#include "data.h"

TemperatureSensor deviceTemp(INTERNAL_TEMPERATURE_PIN, 5);

void temperature_controller_init(void) {
  printf("Temperature controller setup\r\n");
  processor_temperature_init();
  deviceTemp.init();

  if(deviceTemp.isSensorFound()) {
    nectarError.clear_error(DEVICE_OVERHEAT);
  }
}

void temperature_controller_update_processor_temp(void) {
  float processor_temp = processor_temperature_measure();
  // printf("processor temp: %f\r\n", internal_temp);
  if(processor_temp > PROCESSOR_INTERNAL_TEMPERATURE_LIMIT) {
    if(!nectarError.has_error(PROCESSOR_OVERHEAT)) nectarError.set_error(PROCESSOR_OVERHEAT);
    printf("PROCESSOR OVERHEAT\r\n");
  } else {
    if(nectarError.has_error(PROCESSOR_OVERHEAT) && (processor_temp < (PROCESSOR_INTERNAL_TEMPERATURE_LIMIT - 5.0))) nectarError.clear_error(PROCESSOR_OVERHEAT);
  }
}

void temperature_controller_update_internal_temp(void) {
  float internal_temp;
  if(data.safeToReadTemp) {
    data.safeToReadTemp = false;

    if(deviceTemp.isReadyToMeasure()) {
      deviceTemp.measureTemperature();
    }

    if(deviceTemp.isReadyToRead()) {
      deviceTemp.readTemperatureToStorage();
    }

    if(deviceTemp.isNewValueAvailable()) {
      internal_temp = deviceTemp.getTemperature();
      if(internal_temp > DEVICE_TEMPERATURE_LIMIT_MAX) {
        if(!nectarError.has_error(DEVICE_OVERHEAT)) nectarError.set_error(DEVICE_OVERHEAT);
        printf("DEVICE OVERHEAT\r\n");
      } else {
        if(nectarError.has_error(DEVICE_OVERHEAT) && (internal_temp < (DEVICE_TEMPERATURE_LIMIT_MAX - 5.0))) nectarError.clear_error(DEVICE_OVERHEAT);
      }
      data.device_temperature = internal_temp;
    }
  }
}
