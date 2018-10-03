#include "consts.h"
#include "temperature_controller.h"
#include "processor_temperature.h"
#include "ErrorHandler.h"
#include "data.h"

void temperature_controller_init(void) {
  printf("Temperature controller setup\r\n");
  processor_temperature_init();
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
  float internal_temp = 0;
  data.device_temperature = internal_temp;
}
