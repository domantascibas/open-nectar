#include "data.h"
#include "device_modes.h"
#include "ErrorHandler.h"

static const PinName USER_LED = PA_15;
static const PinName OVERHEAT = PD_2;
static const PinName CALIBRATION_BTN = PC_12;

DigitalOut led(USER_LED);
DigitalIn transistorOverheat(OVERHEAT);
InterruptIn calibration_button(CALIBRATION_BTN);
InterruptIn no_power(PC_9); //PC_9 voltage monitor. RESET active - low. normal operation - 1, when <8.5V - 0.

MpptController mppt;
SensorController sensors;

Timer stat_timer;
Ticker update_device_mode;

namespace device_modes {
  static volatile bool update_mode = false;
  volatile bool calibrate_sensors = false;
  
  void calibrate_sensors_ISR() {
    calibrate_sensors = true;
  }
  
  void update_device_mode_ISR() {
    update_mode = true;
  }
  
  void start() {
    data.current_state = RUNNING;
  }
  
  void stop() {
    if(data.current_state != IDLE) {
      data.current_state = STOP;
    }
  }
  
  void enter_service_mode() {
    data.current_state = SERVICE;
  }
  
  void no_power_ISR(){
    if(data.isTestMode == false) {
      sensors.save_meters();
      printf("[ISR] Energy Meters: %.4f, %.4f\r\n", data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
    }
  }
  
  void setup() {
    static const float update_interval = 0.5;
    
    no_power.fall(&no_power_ISR);
    calibration_button.fall(&calibrate_sensors_ISR);
    
    sensors.init();
    mppt.init();
    
    printf("[DEVICE MODES]\r\n");
    update_device_mode.attach(&update_device_mode_ISR, update_interval);
    printf("[ok] interval %.2fs\r\n\n", update_interval);
    stat_timer.start();
  }
  
  void calibrate_device() {
    if(calibrate_sensors) {
      calibrate_sensors = false;
      sensors.calibrate();
    }
  }
  
  void loop() {
    
    if(!nectarError.has_error(CALIBRATION_ERROR)) {
      sensors.measure();
//      led = !led;
    }
    
    if(update_mode) {      
      update_mode = false;
      
      if(nectarError.has_errors && (data.current_state != IDLE)) {
        data.current_state = STOP;
      }
      
      if(data.generator_on && !data.isTestMode) {
        float time_passed = stat_timer.read();
        data.sun_energy_meter_kwh += data.moment_power * time_passed / 3600 / 1000;
      }
      stat_timer.reset();
      
      switch(data.current_state) {
        default:
        case STOP:
          printf("[MODE] stop\r\n");
          mppt.reset();
          data.current_state = IDLE;
          break;
        
        case IDLE:
          printf("[MODE] idle\r\n");
          //wait for start command
          //PWM driver OFF
          break;
        
        case RUNNING:
          printf("[MODE] running\r\n");
          mppt.track();
//          mppt.swipe(0.1, 0.95, 0.1);
          break;
      }
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
