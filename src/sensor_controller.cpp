#include "consts.h"
#include "sensor_controller.h"
#include "Sensor.h"
#include "storage.h"
#include "ErrorHandler.h"
#include "data.h"

float measure_voltage(void);
float measure_current(void);

Sensor voltageSensor(V_SENSE_ADDR);
Sensor currentSensor(I_SENSE_ADDR);

void sensor_controller_init(void) {
  printf("Sensor Controller setup\r\n");
  if(voltageSensor.ping()) nectarError.clear_error(ADC_VOLTAGE_ERROR);
  else nectarError.set_error(ADC_VOLTAGE_ERROR);
  
  if(currentSensor.ping()) nectarError.clear_error(ADC_CURRENT_ERROR);
  else nectarError.set_error(ADC_CURRENT_ERROR);

  if(!nectarError.has_error(FLASH_ACCESS_ERROR)) {
    Storage::load_data(&data.reference_voltage, &data.reference_current, &data.sun_energy_meter_kwh, &data.grid_energy_meter_kwh);
    voltageSensor.set_reference(data.reference_voltage);
    currentSensor.set_reference(data.reference_current);
  } else nectarError.set_error(FLASH_ACCESS_ERROR);
  
  if(!nectarError.has_error(CALIBRATION_ERROR)) {
    if(measure_voltage() < VOLTAGE_LIMIT) nectarError.clear_error(DC_OVER_VOLTAGE);
    else nectarError.set_error(DC_OVER_VOLTAGE);
		nectarError.clear_error(DC_CURRENT_LEAKS);
    if(measure_current() < CURRENT_LIMIT) nectarError.clear_error(DC_OVER_CURRENT);
    else nectarError.set_error(DC_OVER_CURRENT);
  } else nectarError.set_error(CALIBRATION_ERROR);
}

void sensor_controller_measure(void) {
  if(voltageSensor.ready_to_sample) {
    data.moment_voltage = measure_voltage();
  }
  
  if(currentSensor.ready_to_sample) {
    data.moment_current = measure_current();
  }
}

void sensor_controller_calibrate(void) {
  printf("Calibrating\r\n");
  
  voltageSensor.calibrate();
  currentSensor.calibrate();
  
	float c_voltage = voltageSensor.get_reference();
	float c_current = currentSensor.get_reference();
	
	if(((c_voltage < 0.04) && (c_voltage > 0.001)) && ((c_current < 2.2) && (c_current > 1.8))) {
		if(nectarError.has_error(CALIBRATION_ERROR)) {
			nectarError.clear_error(CALIBRATION_ERROR);
		}
		Storage::save_data(c_voltage, c_current);
		data.reference_voltage = c_voltage;	
		data.reference_current = c_current;
		printf("\r\n");
		printf("[ok] calibrated. v_ref = %fV, i_ref = %fA\r\n", c_voltage, c_current);
		printf("\r\n");
	} else {
		printf("\r\n");
		printf("[warn] bad calibration. v_ref = %fV, i_ref = %fA\r\n", c_voltage, c_current);
		printf("*** Please recalibrate with DC+/- inputs shorted ***\r\n");
		if(!nectarError.has_error(CALIBRATION_ERROR)) {
			Storage::load_data(&data.reference_voltage, &data.reference_current, &data.sun_energy_meter_kwh, &data.grid_energy_meter_kwh);
			printf("[warn] loaded last calibration data from memory\r\n");
			printf("[warn] v_ref = %fV, i_ref = %fA\r\n", data.reference_voltage, data.reference_current);
		}
		printf("\r\n");
	}

  sensor_controller_init();
  data.isCalibrating = false;
}

void sensor_controller_save_meters(void) {
  Storage::save_meters(data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
}

float sensor_controller_get_voltage_ref(void) {
  return voltageSensor.get_reference();
}

float sensor_controller_get_current_ref(void) {
  return currentSensor.get_reference();
}

float measure_voltage(void) {
  float v = (voltageSensor.sample() - voltageSensor.get_reference()) / INPUT_VDIV;
  if(v < VOLTAGE_LIMIT && nectarError.has_error(DC_OVER_VOLTAGE)) nectarError.clear_error(DC_OVER_VOLTAGE);
  if(v >= VOLTAGE_LIMIT) nectarError.set_error(DC_OVER_VOLTAGE);
  if(v < 0) v = 0;
  return v;
}

float measure_current(void) {
  float i = (currentSensor.sample() - currentSensor.get_reference()) * 5.000;
  if(i < CURRENT_LIMIT && nectarError.has_error(DC_OVER_CURRENT)) nectarError.clear_error(DC_OVER_CURRENT);
  if(i >= CURRENT_LIMIT) nectarError.set_error(DC_OVER_CURRENT);
  if(i < 0) i = 0;
  return i;
}
