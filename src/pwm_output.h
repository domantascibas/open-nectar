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

// struct PwmController {
//   PwmController(float freq, float min, float max): frequency(freq), duty_min(min), duty_max(max), generator_on(false) {};
    
//   void init();
//   void start();
//   void stop();
//   void set_duty(float);
//   float get_duty();
//   void increase_duty(float);
//   void decrease_duty(float);
//   bool is_on();
  
// private:
//   float frequency;
//   float duty_min;
//   float duty_max;
//   float duty;
//   bool generator_on;

//   float clamp(float);
// };

#endif
