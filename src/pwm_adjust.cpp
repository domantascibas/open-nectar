#include "mbed.h"
#include "pwm_adjust.h"
#include "utils.h"

PwmOut pwm_gen(PB_1);

float       PWM_DUTY = 0.1;
float       PWM_DUTY_STEP_CHANGE = 0.01;
float       old_voltage = 0.0;
float       old_power = 0.0;
bool        reverse = false;
uint8_t     duty_reduce_count = 0;

uint8_t pwm::init(uint16_t frequency = 10) {
    pwm_gen.period_us(1000/frequency);
    pwm_gen.write(PWM_DUTY);
    return PWM_OK;
}

float pwm::set(float voltage, float current) {
    double power;
    float dP;
    float PWM_DUTY_STEP_CHANGE = 0.05;
    
    power = voltage * current;
    dP = power - old_power;
    
    if(dP != 0) {
        //pc.printf("%7.3f\n\r", dP/dV);
        if(dP > 0.5) {
            PWM_DUTY += PWM_DUTY_STEP_CHANGE;
        } else {
            if(duty_reduce_count < 3) {
                PWM_DUTY -= PWM_DUTY_STEP_CHANGE;
                duty_reduce_count++;
            } else {
                PWM_DUTY += PWM_DUTY_STEP_CHANGE;
                duty_reduce_count = 0;
            }
        }
        old_power = power;
    } else {
        PWM_DUTY += PWM_DUTY_STEP_CHANGE;
    }
    
    if(power < 0.5) {
        PWM_DUTY -= 0.25;
    }
    PWM_DUTY = clamp(PWM_DUTY, PWM_MIN, PWM_MAX);
    
    pwm_gen.write(PWM_DUTY);
    return PWM_DUTY;
}

uint8_t pwm::set_pwm(float duty) {
    PWM_DUTY = duty;
    pwm_gen.write(PWM_DUTY);
    return PWM_OK;
}

uint8_t pwm::reset(void) {
    PWM_DUTY = 0.1;
    pwm_gen.write(PWM_DUTY);
    
    //curr_power = 0;
    //curr_voltage = 0;
    //curr_current = 0;
    return PWM_OK;
}


float pwm::get_duty(void) {
    return PWM_DUTY;
}

uint8_t pwm::swipe(float min, float max, float step) {
    if(!reverse) {
        if(PWM_DUTY >= max) {
            reverse = true;
        } else {
            PWM_DUTY += step;
            reverse = false;
        }
    } else {
        if(PWM_DUTY <= min) {
            reverse = false;
        } else {
            PWM_DUTY -= step;
            reverse = true;
        }
    }
    pwm_gen.write(PWM_DUTY);
    return PWM_OK;
}

double pwm::clamp(float input, float min, float max) {
    if(input >= max) {
        return max;
    }
    if(input <= min) {
        return min;
    }
    return input;
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
