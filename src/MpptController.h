#ifndef MPPT_CONTROLLER_H
#define MPPT_CONTROLLER_H

struct MpptController {
  MpptController();
  
  void init();
  void track();
	void stop();
  void reset();
  void swipe(float, float, float);
  float get_duty();
  bool is_generator_on();
  float getDeviceTemperature();
  void updateTemperatures();
  
  void manualIncreaseDuty(bool fineStep = true);
  void manualDecreaseDuty(bool fineStep = true);
  void manualStartPwm();
  void manualStopPwm();
  
private:
  float max_power_point;
  bool last_increase;
  float deviceTemperature;
};

#endif
