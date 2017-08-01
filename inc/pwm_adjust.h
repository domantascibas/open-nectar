#ifndef PWM_ADJUST_H
#define PWM_ADJUST_H

namespace pwm {
    uint8_t init(float);
    
    float adjust();
    uint8_t set(float);
    uint8_t reset(void);
    
    uint8_t swipe(float, float, float);
};

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
