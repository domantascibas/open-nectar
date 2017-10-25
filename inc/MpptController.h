#ifndef MPPT_CONTROLLER_H
#define MPPT_CONTROLLER_H

struct MpptController {
  MpptController();
  
  void init();
  void track();
  void reset();
  void swipe(float, float, float);
  float get_duty();
  bool is_generator_on();
  
private:
  float max_power_point;
  bool last_increase;
};

#endif
