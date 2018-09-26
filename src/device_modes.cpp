#include "consts.h"
#include "data.h"
#include "device_modes.h"
#include "ErrorHandler.h"
#include "power_controller.h"
#include "pwm_controller.h"

DigitalOut led(USER_LED);
DigitalIn transistorOverheat(OVERHEAT);
InterruptIn calibration_button(CALIBRATION_BTN);
InterruptIn no_power(LOW_VOLTAGE_MONITOR); //PC_9 voltage monitor. RESET active - low. normal operation - 1, when <8.5V - 0.

SensorController sensors;

Timer stat_timer;
Ticker update_device_mode;

namespace device_modes {
  static volatile bool update_mode = false;
  volatile bool calibrate_sensors = false;
  float lastPower = 0.0;
  
  void testingStandCalibrate() {
    calibrate_sensors = true;
  }
  
  void calibrate_sensors_ISR() {
    if(data.isInOnboarding) calibrate_sensors = true;
    else calibrate_sensors = false;
  }
  
  void update_device_mode_ISR() {
    update_mode = true;
  }
  
  void start() {
    if(data.current_state != MANUAL) data.current_state = RUNNING;
  }
  
  void stop() {
    if(data.current_state != MANUAL) {
      if(data.current_state != IDLE) {
        data.current_state = STOP;
      }
    }
  }
  
  void no_power_ISR(){
    if(RESET_ENERGY_METERS) {
      data.sun_energy_meter_kwh = 0.00;
      data.grid_energy_meter_kwh = 0.00;
      sensors.save_meters();
      printf("[ISR] Energy Meters: %.4f, %.4f\r\n", data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
    } else {
      if(!data.isTestMode) {
        sensors.save_meters();
        printf("[ISR] Energy Meters: %.4f, %.4f\r\n", data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
      }
    }
  }
  
  void setup() {
    static const float update_interval = 0.5;
    
    no_power.fall(&no_power_ISR);
    calibration_button.fall(&calibrate_sensors_ISR);
    
    sensors.init();
    power_controller_init();
    pwm_controller_init();
    
    printf("[DEVICE MODES]\r\n");
    update_device_mode.attach(&update_device_mode_ISR, update_interval);
    printf("[ok] interval %.2fs\r\n\n", update_interval);
    stat_timer.start();
  }
	
	void runCalibration() {
		data.startCalibration = false;
		calibrate_sensors = false;
		sensors.calibrate();      
		data.sun_energy_meter_kwh = 0.00;
		data.grid_energy_meter_kwh = 0.00;
		sensors.save_meters();
		printf("[ISR] Energy Meters: %.4f, %.4f\r\n", data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
	}
  
  void calibrate_device() {
		if(data.startCalibration) {
			runCalibration();
		}
    if(calibrate_sensors) {
			runCalibration();
    }
  }
  
  void loop() {
    
    if(!nectarError.has_error(CALIBRATION_ERROR)) {
      sensors.measure();
    }

    if(update_mode) {      
      update_mode = false;
      
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
}
