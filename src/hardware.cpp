#include "mbed.h"
#include "hardware.h"
#include "data.h"
#include "DS1820.h"

Ticker update_temperature;
DS1820  probe_boiler(PB_8);
DS1820  probe_internal(PB_9);

extern Data data;

namespace hardware {
    volatile bool update_temp = false;
    
    void update() {
        update_temp = true;
    }
        
    uint8_t setup() {
        //setup temperature sensors
        probe_boiler.begin();
        probe_boiler.startConversion();
        probe_internal.begin();
        probe_internal.startConversion();
        wait(0.5);
        //setup hardware, run self checks
        
        update_temperature.attach(&update, 5.0);
    }
    
    void loop() {
        if(update_temp) {
            update_temp = false;
            __disable_irq();
            data.temp_boiler = probe_boiler.read();
            data.device_temperature = probe_internal.read();
            __enable_irq();
            
            probe_boiler.startConversion();
            probe_internal.startConversion();
            printf("[TEMPERATURE] boiler %.2f\r\n", data.temp_boiler);
        }
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
