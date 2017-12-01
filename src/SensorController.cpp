#include "mbed.h"
#include "SensorController.h"
#include "storage.h"
#include "ErrorHandler.h"
#include "data.h"

const float VOLTAGE_LIMIT = 350.0;
const float CURRENT_LIMIT = 9.0;
const float LEAKAGE_CURRENT = 0.4;

static const uint8_t FILTER_LENGTH = 5;
static const uint8_t V_SENSE_ADDR = 0x55 << 1;
static const uint8_t I_SENSE_ADDR = 0x5A << 1;
static const float INPUT_VDIV = 4.1/400;

Sensor voltageSensor(V_SENSE_ADDR);
Sensor currentSensor(I_SENSE_ADDR);

SensorController::SensorController() {}
  
void SensorController::init() {
  printf("Sensor Controller setup\r\n");
  if(voltageSensor.ping()) nectarError.clear_error(ADC_VOLTAGE_ERROR);
  else nectarError.set_error(ADC_VOLTAGE_ERROR);
  
  if(currentSensor.ping()) nectarError.clear_error(ADC_CURRENT_ERROR);
  else nectarError.set_error(ADC_CURRENT_ERROR);

  Storage::init();
  if(!nectarError.has_error(FLASH_ACCESS_ERROR)) {
    Storage::load_data(&data.reference_voltage, &data.reference_current, &data.sun_energy_meter_kwh, &data.grid_energy_meter_kwh);
    voltageSensor.set_reference(data.reference_voltage);
    currentSensor.set_reference(data.reference_current);
  } else nectarError.set_error(FLASH_ACCESS_ERROR);
  
  if(!nectarError.has_error(CALIBRATION_ERROR)) {
    if(get_voltage() < VOLTAGE_LIMIT) nectarError.clear_error(DC_OVER_VOLTAGE);
    else nectarError.set_error(DC_OVER_VOLTAGE);
    if(get_current() < LEAKAGE_CURRENT) nectarError.clear_error(DC_CURRENT_LEAKS);
    else nectarError.set_error(DC_CURRENT_LEAKS);
    if(get_current() < CURRENT_LIMIT) nectarError.clear_error(DC_OVER_CURRENT);
    else nectarError.set_error(DC_OVER_CURRENT);
  } else nectarError.set_error(CALIBRATION_ERROR);
  
//  lowPassFilter[FILTER_LENGTH] = 0;
//  filterPos = 0;
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
  
  if(v < VOLTAGE_LIMIT && nectarError.has_error(DC_OVER_VOLTAGE)) nectarError.clear_error(DC_OVER_VOLTAGE);
  if(v >= VOLTAGE_LIMIT) nectarError.set_error(DC_OVER_VOLTAGE);
  if(v < 0) v = 0;
  return v;
}

float SensorController::get_current() {
  //float sum, avg;
  float i = (currentSensor.sample() - currentSensor.get_reference()) * 5.000;

  if(i < CURRENT_LIMIT && nectarError.has_error(DC_OVER_CURRENT)) nectarError.clear_error(DC_OVER_CURRENT);
  if(i >= CURRENT_LIMIT) nectarError.set_error(DC_OVER_CURRENT);
  if(i < 0) i = 0;
  
//  lowPassFilter[filterPos] = i;  
//  for(int l=0; l<FILTER_LENGTH; l++) {
//    sum += lowPassFilter[l];
//  }  
//  avg = sum/FILTER_LENGTH;

//  filterPos++;
//  if(filterPos == FILTER_LENGTH) {
//    filterPos = 0;
//  }
//  return avg;
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
  printf("[ok] calibrated. v_ref = %fV, i_ref = %fA\r\n", voltageSensor.get_reference(), currentSensor.get_reference());
  
  init();
  data.isCalibrating = false;
}

void SensorController::save_meters() {
  Storage::save_meters(data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
}
