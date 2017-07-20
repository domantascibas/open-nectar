#ifndef PWM_ADJUST_H
#define PWM_ADJUST_H

namespace pwm {
    uint8_t init(uint16_t);
    
    float set(float, float);
    uint8_t set_pwm(float);
    uint8_t reset(void);
    
    float   get_duty(void);
    
    uint8_t swipe(float, float, float);
    double  clamp(float, float, float);
};

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
