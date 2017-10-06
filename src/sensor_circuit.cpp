#include "mbed.h"
#include "sensor.h"
#include "sensor_circuit.h"
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
//TODO add error handling
    
//    if(voltage >= VOLTAGE_LIMIT) {
//      data.moment_voltage = voltage;
//      data.error = DC_OVER_VOLTAGE;
//      printf("[ERROR] DC Over-Voltage %7.2fV\r\n", data.moment_voltage);
//      return;
//    } else {
//      if(voltage < 0) {
//        data.moment_voltage = 0;
//      } else {
//        data.moment_voltage = voltage;
//      }
//    }
    *value = v;
  }
  
  void get_current(float *value) {
    float i = (i_sensor.sample() - *i_sensor.reference) * 5.000;
//TODO add error handling
    
//    if(current >= CURRENT_LIMIT) {
//      data.moment_current = current;
//      data.error = DC_OVER_CURRENT;
//      printf("[ERROR] DC Over-Current %7.2fA\r\n", data.moment_current);
//      return;
//    } else {
//      if(current < 0) {
//        data.moment_current = 0;
//      } else {
//        data.moment_current = current;
//      }
//    }
    *value = i;
  }  
  
  void init() {
    led = 0;
    v_sensor.ping();
    i_sensor.ping();
//TODO add error handling
    
//    if(adc1_resp != NS_OK) {
//      printf("[ERROR] Voltage ADC not found\r\n");
//      data.error = ADC_VOLTAGE_ERROR;
//      return;
//    }
//    if(adc2_resp != NS_OK) {
//      printf("[ERROR] Current ADC not found\r\n");
//      data.error = ADC_CURRENT_ERROR;
//      return;
//    }
//    printf("[ok] ADC\r\n");
  }
  
  void measure() {
    if(v_sensor.ready_to_sample) {
      get_voltage(&data.moment_voltage);
      DEBUG_PRINT("%fV, ", data.moment_voltage);
      led = !led;
    }
    
    if(i_sensor.ready_to_sample) {
      get_current(&data.moment_current);
      DEBUG_PRINT("%fA\r\n", data.moment_current);
    }
  }
  
  void calibrate() {
    v_sensor.detach_ticker();
    i_sensor.detach_ticker();
    
    printf("Calibrating\r\n");
    printf("v_ref = %fV, i_ref = %fA\r\n", *v_sensor.reference, *i_sensor.reference);
    
    *v_sensor.reference = v_sensor.sample(SAMPLE_NUM << 3);
    *i_sensor.reference = i_sensor.sample(SAMPLE_NUM << 3);
    
    v_sensor.attach_ticker();
    i_sensor.attach_ticker();
    
    printf("[ok] calibrated. v_ref = %fV, i_ref = %fA\r\n", *v_sensor.reference, *i_sensor.reference);
//TODO add error checking
    
//    uint8_t response = NS_OK;
//    device_modes::start();
////    data.reference_voltage = sample(USENSE_ADDR, CALIBRATION_SAMPLES);
////    data.reference_current = sample(ISENSE_ADDR, CALIBRATION_SAMPLES);
//    device_modes::stop();
//    Storage::save_data(data.reference_voltage, data.reference_current, data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
//    if(response != NS_OK) {
//      return response;
//    }
//    return NS_OK;
  }
}
