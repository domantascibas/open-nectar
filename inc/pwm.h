#ifndef PWM_H
#define PWM_H

namespace pwm {
  void setup();
  void loop();

  uint8_t adjust();
  void set();
  void reset();
  void increase_duty();
  void decrease_duty();
  void swipe(float, float, float);
};

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
