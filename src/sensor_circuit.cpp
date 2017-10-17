#include "mbed.h"
#include "sensor.h"
#include "sensor_circuit.h"
#include "error_handler.h"
#include "data.h"

namespace SensorCircuit {
  static const uint8_t V_SENSE_ADDR = 0x55 << 1;
  static const uint8_t I_SENSE_ADDR = 0x5A << 1;
  static const PinName USER_LED = PA_15;
  static const float INPUT_VDIV = 4.1/400;
  
  Sensor v_sensor(V_SENSE_ADDR, &data.reference_voltage);
  Sensor i_sensor(I_SENSE_ADDR, &data.reference_current);
  
  DigitalOut led(USER_LED);
  
  void get_voltage(float *value) {
    float v = (v_sensor.sample() - *v_sensor.reference) / INPUT_VDIV;
    
    if(v >= VOLTAGE_LIMIT) NectarError.set_error(DC_OVER_VOLTAGE);
    if(v < 0) v = 0;
    *value = v;
  }
  
  void get_current(float *value) {
    float i = (i_sensor.sample() - *i_sensor.reference) * 5.000;

    if(i >= CURRENT_LIMIT) NectarError.set_error(DC_OVER_CURRENT);
    if(i < 0) i = 0;
    *value = i;
  }  
  
  void init() {
    led = 0;
    if(!v_sensor.ping()) {
      NectarError.set_error(ADC_VOLTAGE_ERROR);
    }
    if(!i_sensor.ping()) {
      NectarError.set_error(ADC_CURRENT_ERROR);
    }
  }
  
  void measure() {
    if(v_sensor.ready_to_sample) {
      get_voltage(&data.moment_voltage);
//      printf("%fV, ", data.moment_voltage);
      led = !led;
    }
    
    if(i_sensor.ready_to_sample) {
      get_current(&data.moment_current);
//      printf("%fA\r\n", data.moment_current);
    }
  }
  
  void calibrate() {
    v_sensor.detach_ticker();
    i_sensor.detach_ticker();
    
    printf("Calibrating\r\n");
    
    *v_sensor.reference = v_sensor.sample(SAMPLE_NUM << 3);
    *i_sensor.reference = i_sensor.sample(SAMPLE_NUM << 3);
    
    v_sensor.attach_ticker();
    i_sensor.attach_ticker();
    
    printf("[ok] calibrated. v_ref = %fV, i_ref = %fA\r\n", *v_sensor.reference, *i_sensor.reference);
//TODO add error checking
  }
}
