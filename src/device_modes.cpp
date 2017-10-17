#include "mbed.h"
#include "pwm.h"
#include "data.h"
#include "device_modes.h"
#include "storage.h"
#include "error_handler.h"

Timer stat_timer;
Ticker update_device_mode;
InterruptIn no_power(PC_9); //PC_9 voltage monitor. RESET active - low. normal operation - 1, when <8.5V - 0.

namespace device_modes {
  static volatile bool update_mode = false;
  
  void update_device_mode_ISR() {
    update_mode = true;
  }
  
  void start() {
    data.current_state = RUNNING;
  }
  
  void stop() {
    data.current_state = STOP;
  }
  
  void enter_service_mode() {
    data.current_state = SERVICE;
  }
  
  void increase_pwm_duty() {
    pwm::increase_duty();
  }
  
  void decrease_pwm_duty() {
    pwm::decrease_duty();
  }
  
  void no_power_ISR(){
    if(data.isTestMode == false) {
      Storage::save_meters(data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
      printf("[ISR] Energy Meters: %.4f, %.4f\r\n", data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
    }
  }
  
  void setup() {
    static const float update_interval = 0.5;
    
    no_power.fall(&no_power_ISR);
    
    printf("[DEVICE MODES]\r\n");
    update_device_mode.attach(&update_device_mode_ISR, update_interval);
    printf("[ok] interval %.2fs\r\n\n", update_interval);
    stat_timer.start();
  }
  
  void loop() {    
    if(update_mode) {      
      update_mode = false;
      
      if(NectarError.has_error(DC_OVER_CURRENT) || NectarError.has_error(DC_OVER_VOLTAGE)) {
        data.current_state = STOP;
      }
      
      if(data.generator_on && (data.isTestMode == false)) {
        float time_passed = stat_timer.read();
        data.sun_energy_meter_kwh += data.moment_power * time_passed / 3600 / 1000;
      }
      stat_timer.reset();
      
      switch(data.current_state) {
        default:
        case STOP:
          printf("[MODE] stop\r\n");
          pwm::reset();
          data.current_state = IDLE;
          break;
        
        case IDLE:
          printf("[MODE] idle\r\n");
          //wait for start command
          //PWM driver OFF
          break;
        
        case RUNNING:
          printf("[MODE] running\r\n");
          pwm::adjust();
//          pwm::swipe(0.1, 0.95, 0.1);
          break;
        
        case SERVICE:
          printf("[MODE] service\r\n");
          pwm::set();
          break;
      }
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
