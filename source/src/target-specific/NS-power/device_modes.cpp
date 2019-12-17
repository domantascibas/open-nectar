#include "consts.h"
#include "pins.h"
#include "device_modes.h"
#include "error_controller.h"
#include "power_controller.h"
#include "pwm_controller.h"
#include "temperature_controller.h"
#include "sensor_controller.h"

extern "C" {
    #include "data.h"
}

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
  uint32_t s, g;
  PowerData_read(SUN_METER, &s);
  PowerData_read(GRID_METER, &g);
  printf("Energy Meters: %u, %u\r\n", (unsigned int)s, (unsigned int)g);
}

void device_modes_loop(void) {
  // if(button_pressed) {
  //   button_pressed = false;
  //   printf("!! calibration button pressed !!\r\n");
  // }

  if(data_getStartCalibration()) {
    data_setStartCalibration(0);
    calibrate();
  }

  if(!nectarError_has_error(CALIBRATION_ERROR)) {
    sensor_controller_measure();
  }

  if(update_mode) {      
    update_mode = false;

    temperature_controller_update_processor_temp();
    temperature_controller_update_internal_temp();
    
    if(nectarError_has_errors() && ((data_getCurrent_state() != (uint8_t)IDLE) || (data_getCurrent_state() != (uint8_t)MANUAL))) {
      data_setCurrent_state((uint8_t)STOP);
    }
    
    if(power_controller_is_generator_on() && !data_getIsTestMode()) {
      uint32_t s, p;
      float time_passed = stat_timer.read();
      stat_timer.reset();
      PowerData_read(SUN_METER, &s);
      PowerData_read(M_POWER, &p);
      if(time_passed > 0) s += ((lastPower + p)/2 )* time_passed / 3600 / 1000;
      // lastPower = data.moment_power;
      lastPower = p;
    } else {
      stat_timer.reset();
      lastPower = 0.0;
    }
    
    if(data_getCurrent_state() != (uint8_t)MANUAL) {
      switch(data_getCurrent_state()) {
        default:
        case STOP:
          printf("[MODE] stop\r\n");
          power_controller_mppt_stop();
          data_setCurrent_state((uint8_t)IDLE);
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
  if(data_getCurrent_state() != (uint8_t)MANUAL) data_setCurrent_state((uint8_t)RUNNING);
}

void device_modes_set_state_stop(void) {
  if(data_getCurrent_state() != (uint8_t)MANUAL) {
    if(data_getCurrent_state() != (uint8_t)IDLE) {
      data_setCurrent_state((uint8_t)STOP);
    }
  }
}

void calibrate(void) {
  sensor_controller_calibrate();
  uint32_t s, g = 0;
  PowerData_write(SUN_METER, &s);
  PowerData_write(GRID_METER, &g);
  // data.sun_energy_meter_kwh = 0.00;
  // data.grid_energy_meter_kwh = 0.00;
  sensor_controller_save_meters();
  printf("[ISR] Energy Meters: %u, %u\r\n", (unsigned int)s, (unsigned int)g);
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
  if(!data_getIsTestMode()) {
    uint32_t s, g;
    PowerData_read(SUN_METER, &s);
    PowerData_read(GRID_METER, &g);
    sensor_controller_save_meters();
    device_modes_set_state_stop();
    printf("[ISR] Energy Meters: %u, %u\r\n", (unsigned int)s, (unsigned int)g);
  }
}
