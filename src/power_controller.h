#ifndef _NS_POWER_CONTROLLER_H
#define _NS_POWER_CONTROLLER_H

void power_controller_init(void);
void power_controller_mppt_stop(void);
void power_controller_mppt_start(void);
void power_controller_mppt_restart(void);
void power_controller_swipe(void);

float power_controller_get_duty(void);
bool power_controller_is_generator_on(void);

void power_controller_manual_increase_duty(bool fine = true);
void power_controller_manual_decrease_duty(bool fine = true);
void power_controller_manual_output_on(void);
void power_controller_manual_output_off(void);

// struct MpptController {
//   MpptController();
  
//   void init();
//   void track();
// 	void stop();
//   void reset();
//   void swipe(float, float, float);
//   float get_duty();
//   bool is_generator_on();
//   float getDeviceTemperature();
//   void updateTemperatures();
  
//   void manualIncreaseDuty(bool fineStep = true);
//   void manualDecreaseDuty(bool fineStep = true);
//   void manualStartPwm();
//   void manualStopPwm();
  
// private:
//   float max_power_point;
//   bool last_increase;
//   float deviceTemperature;
// };

#endif
