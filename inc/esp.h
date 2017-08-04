#ifndef ESP_H
#define ESP_H

namespace esp {
    void init(uint32_t);
    
    void loop(void);
    
    //reads message from ESP
    //sets the temperature_max, temperature_min, temperature_scheduled and current_mode
    uint8_t set_mode(void);
    
    uint8_t cancel_boost(void);
}

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
