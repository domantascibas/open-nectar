#include "mbed.h"
#include "pwm_adjust.h"
#include "data.h"

PwmOut pwm_gen(PB_1);

float       PWM_DUTY_STEP_CHANGE = 0.02;
float       old_voltage = 0.0;
float       old_power = 0.0;
bool        reverse = false;
uint8_t     duty_reduce_count = 0;

namespace pwm {
    float clamp(float, float, float);
    
    uint8_t init(float frequency = 10) {
        pwm_gen.period_us(1000/frequency);
        pwm_gen.write(data.pwm_duty);
        return NS_OK;
    }
    
    uint8_t adjust() {
        float pwm_duty = data.pwm_duty;
        float dP;
        
        dP = data.moment_power - old_power;
        
        if(dP != 0) {
            //pc.printf("%7.3f\n\r", dP/dV);
            if(dP > 0.1) {
                pwm_duty += PWM_DUTY_STEP_CHANGE;
            } else {
                if(duty_reduce_count < 3) {
                    pwm_duty -= PWM_DUTY_STEP_CHANGE;
                    duty_reduce_count++;
                } else {
                    pwm_duty += PWM_DUTY_STEP_CHANGE;
                    duty_reduce_count = 0;
                }
            }
            old_power = data.moment_power;
        } else {
            pwm_duty += PWM_DUTY_STEP_CHANGE;
        }
        
        if(data.moment_power < 0.5) {
            pwm_duty -= 0.25;
        }
        pwm_duty = clamp(pwm_duty, PWM_MIN, PWM_MAX);
        
        pwm_gen.write(pwm_duty);
        data.pwm_duty = pwm_duty;
        return NS_OK;
    }
    
    uint8_t set() {
        pwm_gen.write(data.pwm_duty);
        return NS_OK;
    }
    
    uint8_t reset(void) {
        data.pwm_duty = 0.1;
        pwm_gen.write(data.pwm_duty);
        return NS_OK;
    }
    
    uint8_t swipe(float min, float max, float step) {
        float pwm_duty = data.pwm_duty;
        if(!reverse) {
            if(pwm_duty >= max) {
                reverse = true;
            } else {
                pwm_duty += step;
                reverse = false;
            }
        } else {
            if(pwm_duty <= min) {
                reverse = false;
            } else {
                pwm_duty -= step;
                reverse = true;
            }
        }
        pwm_gen.write(pwm_duty);
        data.pwm_duty = pwm_duty;
        return PWM_OK;
    }
    
    float clamp(float input, float min, float max) {
        if(input >= max) {
            return max;
        }
        if(input <= min) {
            return min;
        }
        return input;
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
