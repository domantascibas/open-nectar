#include "mbed.h"
#include "DS1820.h"
#include "data.h"
#include "hardware.h"

Ticker update_temperature;
DS1820 probe_boiler(PB_8);
DS1820 probe_internal(PB_9);

namespace hardware {
  volatile bool update_temp = false;
  bool available = true;

  void update() {
    update_temp = true;
  }

  uint8_t setup() {
//    if(probe_boiler.isPresent()) {
      probe_boiler.begin();
      probe_boiler.startConversion();
//      if(probe_internal.isPresent()) {
        probe_internal.begin();
        probe_internal.startConversion();

        wait(0.5);
        update_temperature.attach(&update, 5.0);
//      } else {
//        data.error = INTERNAL_TEMP_SENSOR_NOT_PRESENT;
//      }
//    } else {
//      data.error = WATER_TEMP_SENSOR_NOT_PRESENT;
//    }
  }

  void loop() {
    if(update_temp && available) {
      update_temp = false;
      
      __disable_irq();
      probe_boiler.read(data.temp_boiler);
      probe_boiler.startConversion();
      
      probe_internal.read(data.device_temperature);
      probe_internal.startConversion();
      __enable_irq();

      printf("[TEMPERATURE] boiler %.2f\r\n", data.temp_boiler);
      printf("[TEMPERATURE] internal %.2f\r\n", data.device_temperature);
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
