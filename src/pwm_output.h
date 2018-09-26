#ifndef _NS_PWM_OUTPUT_H
#define _NS_PWM_OUTPUT_H

void pwm_generator_init(void);
void pwm_generator_start(void);
void pwm_generator_stop(void);
void pwm_generator_increase_duty(void);
void pwm_generator_decrease_duty(void);
void pwm_generator_set_duty(float value);

float pwm_generator_get_duty(void);
bool pwm_generator_is_on(void);

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
