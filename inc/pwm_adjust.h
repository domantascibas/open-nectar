#ifndef PWM_ADJUST_H
#define PWM_ADJUST_H

namespace pwm {
    uint8_t init(uint16_t);
    
    uint8_t set(double, double);
    uint8_t set_pwm(float);
    uint8_t reset(void);
    
    
    float   getDuty(void);
    
    uint8_t swipe(float, float, float);
    double  clamp(float, float, float);
};

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
