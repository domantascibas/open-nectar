#include "mbed.h"
#include "DS1820.h"
#include "data.h"
#include "hardware.h"

static const PinName DEVICE_THERMO = PC_6;
static const PinName RADIATOR_THERMO = PC_7;

Ticker update_temperature;
DS1820 probe_device(DEVICE_THERMO);
DS1820 probe_radiator(RADIATOR_THERMO);

namespace hardware {
  volatile bool update_temp = false;
  
  void update() {
    update_temp = true;
  }
  
  void setup() {
    static const float update_interval = 5.0;
    
    printf("[TEMPERATURE]\r\n");
    probe_device.begin();
    probe_device.startConversion();
    probe_radiator.begin();
    probe_radiator.startConversion();
    wait(0.5);
    
    //TODO setup hardware, run self checks
    
    update_temperature.attach(&update, update_interval);
    printf("[ok] interval %.2fs\r\n\n", update_interval);
  }
  
  void loop() {
    if(update_temp) {
      update_temp = false;
      
      __disable_irq();
      data.airgap_temperature = probe_device.read();
      data.radiator_temperature = probe_radiator.read();
      __enable_irq();
      
      probe_device.startConversion();
      probe_radiator.startConversion();
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
