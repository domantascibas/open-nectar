#include "consts.h"
#include "sensor_controller.h"
#include "Sensor.h"
#include "flash_storage.h"
#include "error_controller.h"
#include "data.h"

float measure_voltage(void);
float measure_current(void);

Sensor voltageSensor(V_SENSE_ADDR);
Sensor currentSensor(I_SENSE_ADDR);

void sensor_controller_init(void) {
  uint16_t v_ref, i_ref;
  printf("Sensor Controller setup\r\n");
  if(voltageSensor.ping()) nectarError.clear_error(ADC_VOLTAGE_ERROR);
  else nectarError.set_error(ADC_VOLTAGE_ERROR);
  
  if(currentSensor.ping()) nectarError.clear_error(ADC_CURRENT_ERROR);
  else nectarError.set_error(ADC_CURRENT_ERROR);

  if(!nectarError.has_error(FLASH_ACCESS_ERROR)) {
    // data.calibrated = flash_storage_load_data(&data.reference_voltage, &data.reference_current, &data.sun_energy_meter_kwh, &data.grid_energy_meter_kwh);
    flash_storage_load_data();

    PowerData_read(R_VOLTAGE, &v_ref);
    PowerData_read(R_CURRENT, &i_ref);

    voltageSensor.set_reference(((float)v_ref) / 100);
    currentSensor.set_reference(((float)i_ref) / 100);
  } else nectarError.set_error(FLASH_ACCESS_ERROR);
  
  if(GET_STATUS(CALIBRATION_STATUS)) {
    nectarError.clear_error(CALIBRATION_ERROR);
  }

  if(!nectarError.has_error(CALIBRATION_ERROR)) {
    if(measure_voltage() < VOLTAGE_LIMIT) nectarError.clear_error(DC_OVER_VOLTAGE);
    else nectarError.set_error(DC_OVER_VOLTAGE);
		nectarError.clear_error(DC_CURRENT_LEAKS);
    if(measure_current() < CURRENT_LIMIT) nectarError.clear_error(DC_OVER_CURRENT);
    else nectarError.set_error(DC_OVER_CURRENT);
  } else nectarError.set_error(CALIBRATION_ERROR);
}

void sensor_controller_measure(void) {
  uint16_t v, i;
  if(voltageSensor.ready_to_sample) {
    v = (uint16_t)(measure_voltage() * 100);
    PowerData_write(M_VOLTAGE, &v);
    // data.momnt_voltage = measure_voltage();
  }
  
  if(currentSensor.ready_to_sample) {
    i = (uint16_t)(measure_current() * 100);
    PowerData_write(M_CURRENT, &i);
    // data.moment_current = measure_current();
  }
}

void sensor_controller_calibrate(void) {
  printf("Calibrating\r\n");
  
  voltageSensor.calibrate();
  currentSensor.calibrate();
  
	uint16_t c_voltage = (uint16_t)(voltageSensor.get_reference() * 100);
	uint16_t c_current = (uint16_t)(currentSensor.get_reference() * 100);
	
	if(((c_voltage < CALIBRATION_VOLTAGE_MAX * 100) && (c_voltage > CALIBRATION_VOLTAGE_MIN * 100)) && ((c_current < CALIBRATION_CURRENT_MAX * 100) && (c_current > CALIBRATION_CURRENT_MIN * 100))) {
		if(nectarError.has_error(CALIBRATION_ERROR)) {
			nectarError.clear_error(CALIBRATION_ERROR);
		}
		// flash_storage_save_data(c_voltage, c_current);
		flash_storage_save_data();

		// data.reference_voltage = c_voltage;
		// data.reference_current = c_current;
    PowerData_write(R_VOLTAGE, &c_voltage);
    PowerData_write(R_CURRENT, &c_current);
		printf("\r\n");
		printf("[ok] calibrated. v_ref = %dV, i_ref = %dA\r\n", c_voltage, c_current);
		printf("\r\n");
	} else {
		printf("\r\n");
		printf("[warn] bad calibration. v_ref = %dV, i_ref = %dA\r\n", c_voltage, c_current);
		printf("*** Please recalibrate with DC+/- inputs shorted ***\r\n");
		if(!nectarError.has_error(CALIBRATION_ERROR)) {
			// flash_storage_load_data(&data.reference_voltage, &data.reference_current, &data.sun_energy_meter_kwh, &data.grid_energy_meter_kwh);
			flash_storage_load_data();
			printf("[warn] loaded last calibration data from memory\r\n");
      PowerData_read(R_VOLTAGE, &c_voltage);
      PowerData_read(R_CURRENT, &c_current);
			printf("[warn] v_ref = %dV, i_ref = %dA\r\n", c_voltage, c_current);
		}
		printf("\r\n");
	}

  sensor_controller_init();
  data.isCalibrating = false;
}

void sensor_controller_save_meters(void) {
  flash_storage_save_meters();
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
