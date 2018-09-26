#ifndef _NS_PWM_OUTPUT_H
#define _NS_PWM_OUTPUT_H

void pwm_output_init(void);
void pwm_output_start(void);
void pwm_output_stop(void);
void pwm_output_increase_duty(void);
void pwm_output_decrease_duty(void);
void pwm_output_increase_duty_fine(void);
void pwm_output_decrease_duty_fine(void);
void pwm_output_set_duty(float value);

float pwm_output_get_duty(void);
bool pwm_output_is_on(void);

#endif
