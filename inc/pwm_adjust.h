#ifndef PWM_ADJUST_H
#define PWM_ADJUST_H

namespace pwm {
    int     init(int);
    
    int     set(double, double);
    int     reset();
    
    float   getDuty();
    
    int     swipe(float, float, float);
    double  clamp(float, float, float);
};

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
