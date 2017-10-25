#include "mbed.h"
#include "SensorController.h"
#include "storage.h"
#include "ErrorHandler.h"
#include "data.h"

static const uint8_t V_SENSE_ADDR = 0x55 << 1;
static const uint8_t I_SENSE_ADDR = 0x5A << 1;
static const float INPUT_VDIV = 4.1/400;

Sensor voltageSensor(V_SENSE_ADDR);
Sensor currentSensor(I_SENSE_ADDR);

SensorController::SensorController() {}
  
void SensorController::init() {
  printf("Sensor Controller setup\r\n");
  if(voltageSensor.ping()) {
    nectarError.clear_error(ADC_VOLTAGE_ERROR);
  }
  if(currentSensor.ping()) {
    nectarError.clear_error(ADC_CURRENT_ERROR);
  }

  Storage::init();
  if(!nectarError.has_errors) {
    Storage::load_data(&data.reference_voltage, &data.reference_current, &data.sun_energy_meter_kwh, &data.grid_energy_meter_kwh);
    voltageSensor.set_reference(data.reference_voltage);
    currentSensor.set_reference(data.reference_current);
    data.calibrated = true;
  } else {
    data.calibrated = false;
  }
}

void SensorController::measure() {
  if(voltageSensor.ready_to_sample) {
    data.moment_voltage = get_voltage();
//    printf("%fV, ", data.moment_voltage);
  }
  
  if(currentSensor.ready_to_sample) {
    data.moment_current = get_current();
//    printf("%fA\r\n", data.moment_current);
  }
}

float SensorController::get_voltage() {
  float v = (voltageSensor.sample() - voltageSensor.get_reference()) / INPUT_VDIV;
    
  if(v >= VOLTAGE_LIMIT) nectarError.set_error(DC_OVER_VOLTAGE);
  if(v < 0) v = 0;
  return v;
}

float SensorController::get_current() {
  float i = (currentSensor.sample() - currentSensor.get_reference()) * 5.000;

  if(i >= CURRENT_LIMIT) nectarError.set_error(DC_OVER_CURRENT);
  if(i < 0) i = 0;
  return i;
}

float SensorController::get_voltage_reference() {
  return voltageSensor.get_reference();
}

float SensorController::get_current_reference() {
  return currentSensor.get_reference();
}

void SensorController::calibrate() {
  if(nectarError.has_error(CALIBRATION_ERROR)) {
    nectarError.clear_error(CALIBRATION_ERROR);
  }
  printf("Calibrating\r\n");
  
  voltageSensor.calibrate();
  currentSensor.calibrate();
  
  Storage::save_data(voltageSensor.get_reference(), currentSensor.get_reference());
  data.reference_current = currentSensor.get_reference();
  data.reference_voltage = voltageSensor.get_reference();
  data.calibrated = true;
  printf("[ok] calibrated. v_ref = %fV, i_ref = %fA\r\n", voltageSensor.get_reference(), currentSensor.get_reference());
  //TODO add error checking
}

void SensorController::save_meters() {
  Storage::save_meters(data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
}