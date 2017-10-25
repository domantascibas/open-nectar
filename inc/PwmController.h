#ifndef PWM_CONTROLLER_H
#define PWM_CONTROLLER_H

#define DRIVER_ON         0
#define DRIVER_OFF        1

struct PwmController {
  PwmController(float freq, float min, float max): frequency(freq), duty_min(min), duty_max(max), generator_on(false) {};
    
  void init();
  void start();
  void stop();
  void set_duty(float);
  float get_duty();
  void increase_duty(float);
  void decrease_duty(float);
  bool is_on();
  
private:
  float frequency;
  float duty_min;
  float duty_max;
  float duty;
  bool generator_on;

  float clamp(float);
};

#endif
