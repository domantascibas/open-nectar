#include "consts.h"
#include "data.h"
#include "device_modes.h"
#include "error_controller.h"
#include "power_controller.h"
#include "pwm_controller.h"
#include "temperature_controller.h"
#include "sensor_controller.h"

void calibrate(void);
// void calibrate_ISR(void);
void update_mode_ISR(void);
void shutdown_ISR(void);

// static volatile bool button_pressed = false;
static volatile bool update_mode = false;
static float lastPower = 0.0;

Timer stat_timer;
Ticker update_device_mode;
DigitalOut led(USER_LED);
DigitalIn transistorOverheat(OVERHEAT);
// InterruptIn calibration_button(CALIBRATION_BTN);
InterruptIn no_power(LOW_VOLTAGE_MONITOR); //PC_9 voltage monitor. RESET active - low. normal operation - 1, when <8.5V - 0.

void device_modes_init(void) {
  no_power.fall(&shutdown_ISR);
  // calibration_button.fall(&calibrate_ISR);

  temperature_controller_init();
  sensor_controller_init();
  power_controller_init();
  pwm_controller_init();
  
  printf("[DEVICE MODES]\r\n");
  update_device_mode.attach(&update_mode_ISR, DEVICE_MODE_UPDATE_INTERVAL);
  printf("[ok] interval %.2fs\r\n\n", DEVICE_MODE_UPDATE_INTERVAL);
  stat_timer.start();
  printf("Energy Meters: %.4f, %.4f\r\n", data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
}

void device_modes_loop(void) {
  // if(button_pressed) {
  //   button_pressed = false;
  //   printf("!! calibration button pressed !!\r\n");
  // }

  if(data.startCalibration) {
    data.startCalibration = false;
    calibrate();
  }

  if(!nectarError.has_error(CALIBRATION_ERROR)) {
    sensor_controller_measure();
  }

  if(update_mode) {      
    update_mode = false;

    temperature_controller_update_processor_temp();
    temperature_controller_update_internal_temp();
    
    if(nectarError.has_errors && ((data.current_state != IDLE) || (data.current_state != MANUAL))) {
      data.current_state = STOP;
    }
    
    if(power_controller_is_generator_on() && !data.isTestMode) {
      float time_passed = stat_timer.read();
      stat_timer.reset();
      if(time_passed > 0) data.sun_energy_meter_kwh += ((lastPower + data.moment_power)/2 )* time_passed / 3600 / 1000;
      lastPower = data.moment_power;
    } else {
      stat_timer.reset();
      lastPower = 0.0;
    }
    
    if(data.current_state != MANUAL) {
      switch(data.current_state) {
        default:
        case STOP:
          printf("[MODE] stop\r\n");
          power_controller_mppt_stop();
          data.current_state = IDLE;
          break;
        
        case IDLE:
          printf("[MODE] idle\r\n");
          //wait for start command
          //PWM driver OFF
          break;
        
        case RUNNING:
          printf("[MODE] running\r\n");
          power_controller_mppt_start();
//          power_controller_mppt_swipe();
          break;
        
        case MANUAL:
          printf("[MODE] manual\r\n");
          break;
      }
    }
  }
}

void device_modes_set_state_running(void) {
  if(data.current_state != MANUAL) data.current_state = RUNNING;
}

void device_modes_set_state_stop(void) {
  if(data.current_state != MANUAL) {
    if(data.current_state != IDLE) {
      data.current_state = STOP;
    }
  }
}

void calibrate(void) {
  sensor_controller_calibrate();      
  data.sun_energy_meter_kwh = 0.00;
  data.grid_energy_meter_kwh = 0.00;
  sensor_controller_save_meters();
  printf("[ISR] Energy Meters: %.4f, %.4f\r\n", data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
}

// void calibrate_ISR(void) {
//   // button_pressed = true;
//   if(data.isInOnboarding | data.isTestMode) data.startCalibration = true;
//   else data.startCalibration = false;
// }

void update_mode_ISR(void) {
  update_mode = true;
}

void shutdown_ISR(void) {
  if(!data.isTestMode) {
    sensor_controller_save_meters();
    printf("[ISR] Energy Meters: %.4f, %.4f\r\n", data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
  }
}
