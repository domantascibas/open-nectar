#include "consts.h"
#include "pins.h"
#include "sensor_controller.h"
#include "Sensor.h"
// #include "error_controller.h"
#include "flash_storage.h"
#include "error_handler.h"

extern "C" {
    #include "data.h"
}

float measure_voltage(void);
float measure_current(void);

Sensor voltageSensor(V_SENSE_ADDR);
Sensor currentSensor(I_SENSE_ADDR);

void sensor_controller_init(void) {
  uint16_t ref;
  printf("Sensor Controller setup\r\n");
  if (voltageSensor.ping()) {
    error_clear(NS_ADC_VOLTAGE);
  } else {
    error_set(NS_ADC_VOLTAGE);
  }
  
  if (currentSensor.ping()) {
    error_clear(NS_ADC_CURRENT);
  } else {
    error_set(NS_ADC_CURRENT);
  }

  if (!error_isSet(NS_FLASH_ACCESS_POWER)) {
    // data.calibrated = flash_storage_load_data(&data.reference_voltage, &data.reference_current, &data.sun_energy_meter_kwh, &data.grid_energy_meter_kwh);
    flash_storage_load_data();

    PowerData_read(R_VOLTAGE, &ref);
    voltageSensor.set_reference(DIV_REF_CONVERT(ref));
    PowerData_read(R_CURRENT, &ref);
    currentSensor.set_reference(DIV_REF_CONVERT(ref));
  } else {
    error_set(NS_FLASH_ACCESS_POWER);
  }
  
  if (GET_STATUS(CALIBRATION_STATUS)) {
    error_clear(NS_CALIBRATION);
  }

  if (!error_isSet(NS_CALIBRATION)) {
    if (measure_voltage() < VOLTAGE_LIMIT) {
      error_clear(NS_DC_OVER_VOLTAGE);
    } else {
      error_set(NS_DC_OVER_VOLTAGE);
    }
    
    error_clear(NS_DC_CURRENT_LEAKS);

    if (measure_current() < CURRENT_LIMIT) {
      error_clear(NS_DC_OVER_CURRENT);
    } else {
      error_set(NS_DC_OVER_CURRENT);
    }
  } else {
    error_set(NS_CALIBRATION);
  }
}

void sensor_controller_measure(void) {
  uint16_t vi;
  if(voltageSensor.ready_to_sample) {
    vi = VI_CONVERT(measure_voltage());
    PowerData_write(M_VOLTAGE, &vi);
    // data.momnt_voltage = measure_voltage();
  }
  
  if(currentSensor.ready_to_sample) {
    vi = VI_CONVERT(measure_current());
    PowerData_write(M_CURRENT, &vi);
    // data.moment_current = measure_current();
  }
}

void sensor_controller_calibrate(void) {
  printf("Calibrating\r\n");
  
  voltageSensor.calibrate();
  currentSensor.calibrate();
  
	uint16_t c_voltage = REF_CONVERT(voltageSensor.get_reference());
	uint16_t c_current = REF_CONVERT(currentSensor.get_reference());
	
	if(((c_voltage < CALIBRATION_VOLTAGE_MAX * 100) && (c_voltage > CALIBRATION_VOLTAGE_MIN * 100)) && ((c_current < CALIBRATION_CURRENT_MAX * 100) && (c_current > CALIBRATION_CURRENT_MIN * 100))) {
		if (error_isSet(NS_CALIBRATION)) {
			error_clear(NS_CALIBRATION);
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
		if (!error_isSet(NS_CALIBRATION)) {
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
  data_setIsCalibrating(0);
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
  if (v < VOLTAGE_LIMIT && error_isSet(NS_DC_OVER_VOLTAGE)) {
    error_clear(NS_DC_OVER_VOLTAGE);
  }
  if (v >= VOLTAGE_LIMIT) {
    error_set(NS_DC_OVER_VOLTAGE);
  }
  if (v < 0) {
    v = 0;
  }
  return v;
}

float measure_current(void) {
  float i = (currentSensor.sample() - currentSensor.get_reference()) * 5.000;
  if (i < CURRENT_LIMIT && error_isSet(NS_DC_OVER_CURRENT)) {
    error_clear(NS_DC_OVER_CURRENT);
  }
  if (i >= CURRENT_LIMIT) {
    error_set(NS_DC_OVER_CURRENT);
  }
  if (i < 0) {
    i = 0;
  }
  return i;
}
