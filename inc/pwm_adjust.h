#ifndef PWM_ADJUST_H
#define PWM_ADJUST_H

namespace pwm {
    uint8_t init(float);
    
    uint8_t adjust();
    uint8_t set();
    uint8_t reset(void);
    
    uint8_t swipe(float, float, float);
};

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
