#include "mbed.h"
#include "measurements.h"
#include "storage.h"
#include "device_modes.h"
#include "sensor_circuit.h"
#include "error_handler.h"
#include "data.h"

static const PinName CALIBRATION_BTN = PC_12;
InterruptIn calibration_button(CALIBRATION_BTN);

//SENSORS
namespace sensors {
  volatile bool calibrate_sensors = false;
  
  void calibrate_sensors_ISR() {
    calibrate_sensors = true;
  }
  
  void setup() {
    calibration_button.fall(&calibrate_sensors_ISR);
    
    printf("[MEASUREMENTS]\r\n");
    SensorCircuit::init();
    
    if(!NectarError.has_errors) {
      Storage::init();
      if(!NectarError.has_errors) {
        Storage::load_data(&data.reference_voltage, &data.reference_current, &data.sun_energy_meter_kwh, &data.grid_energy_meter_kwh);
        data.calibrated = true;
      } else {
        data.calibrated = false;
      }
    }
    DEBUG_PRINT("calib: 0x%X, vref: %f, iref: %f, sun meter: %f, grid meter: %f\r\n", data.calibrated, data.reference_voltage, data.reference_current, data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
   
//    //measure Vdc < 350
//    get_voltage();
//    if(data.error != 0x00) return;
//    printf("[ok] V = %7.2fV\r\n", data.moment_voltage);
//    
//    //measure Idc == ~0.0
//    get_current();
//    if(data.error != 0x00) return;
//    if(data.moment_current > 0.2) {
//      printf("[ERROR] DC Current Leaks %7.2fA\r\n", data.moment_current);
//      data.error = DC_CURRENT_LEAKS;
//      return;
//    }
//    printf("[ok] I = %7.2fA\r\n", data.moment_current);
//    
//    //check Tcap and Tmosfet
//    //TODO
//    
//    //check PWM generator
//    //TODO
//    
//    measure_voltage.attach(&measure_voltage_ISR, update_interval);
//    measure_current.attach(&measure_current_ISR, update_interval);
//    printf("[ok] interval %.2fs\r\n\n", update_interval);
  }
  
  void loop() {
    if(!NectarError.has_error(CALIBRATION_ERROR)) {
      SensorCircuit::measure();
    }
    
    if(calibrate_sensors) {
      if(NectarError.has_error(CALIBRATION_ERROR)) {
        NectarError.clear_error(CALIBRATION_ERROR);
      }
      data.calibrated = true;
      calibrate_sensors = false;
      SensorCircuit::calibrate();
      Storage::save_data(data.reference_voltage, data.reference_current);
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
