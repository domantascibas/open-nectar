#include "mbed.h"
#include "pwm_adjust.h"
#include "utils.h"

PwmOut pwm_gen(PC_6);

double		PWM_DUTY = 0.1;
double 		PWM_DUTY_STEP_CHANGE = 0.01;
double 		old_voltage = 0.0;
double 		old_power = 0.0;
bool 		reverse = false;
int			duty_reduce_count = 0;

int pwm::init(int frequency = 10000) {
	pwm_gen.period_us(1000/frequency);
    pwm_gen.write(PWM_DUTY);
	return PWM_OK;
}

int pwm::set(double voltage, double current) {
	double power;
	double dP;
	double PWM_DUTY_STEP_CHANGE = 0.05;
	
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
	} else PWM_DUTY += PWM_DUTY_STEP_CHANGE;
	
	if(power < 0.5) PWM_DUTY -= 0.25;
	PWM_DUTY = clamp(PWM_DUTY, PWM_MIN, PWM_MAX);
	
	pwm_gen.write(PWM_DUTY);
	return PWM_OK;
}

int pwm::reset(void) {
	PWM_DUTY = 0.1;
	pwm_gen.write(PWM_DUTY);
	
	//curr_power = 0;
	//curr_voltage = 0;
	//curr_current = 0;
	return PWM_OK;
}

int pwm::swipe(float min, float max, float step) {
	
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
	return PWM_OK;
}

double pwm::clamp(float input, float min, float max) {
	if(input >= max) return max;
	if(input <= min) return min;
	return input;
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
