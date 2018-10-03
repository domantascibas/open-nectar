#ifndef _NS_PWM_CONTROLLER_H
#define _NS_PWM_CONTROLLER_H

void pwm_controller_init(void);
void pwm_controller_increase_duty(float step);
void pwm_controller_decrease_duty(float step);
void pwm_controller_set_duty(float value);
float pwm_controller_get_duty(void);

#endif
