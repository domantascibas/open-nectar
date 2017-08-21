#include "mbed.h"
#include "pwm.h"
#include "data.h"
#include "device_modes.h"

Ticker update_device_mode;

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
  
  void setup() {
    static const float update_interval = 0.5;
    
    printf("[DEVICE MODES]\r\n");
    update_device_mode.attach(&update_device_mode_ISR, update_interval);
    printf("[ok] interval %.2fs\r\n\n", update_interval);
  }
  
  void loop() {
    if(update_mode) {
      update_mode = false;
      //uint8_t current_state = data.current_state;
      //printf("Current State %d\r\n", data.current_state);
      
      switch(data.current_state) {
        default:
        case STOP:
          pwm::reset();
          data.current_state = IDLE;
        break;
        
        case IDLE:
          //wait for start command
          //PWM driver OFF
        break;
        
        case RUNNING:
          pwm::adjust();
//          pwm::swipe(0.1, 0.95, 0.1);
        break;
        
        case SERVICE:
          pwm::set();
        break;
      }
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
