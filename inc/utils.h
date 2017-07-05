#ifndef UTILS_H
#define UTILS_H

//Global constants
#define PWM_MIN					0.1
#define PWM_MAX					0.95
#define	V_REF					2.500

#define OVERVOLTAGE				350.0
#define OVERCURRENT				10.0

enum error_codes{
	OK,
	SETUP_OK,
	SETUP_ERROR,
	STARTUP_OK,
	STARTUP_ERROR,
	ADC_OK,
	ADC_ERROR,
	ADC_SETUP_OK,
	ADC_SETUP_ERROR,
	FLASH_ACCESS_OK,
	FLASH_ACCESS_ERROR,
	FLASH_READ_OK,
	FLASH_READ_ERROR,
	FLASH_WRITE_OK,
	FLASH_WRITE_ERROR,
	CALIBRATION_OK,
	CALIBRATION_ERROR,
	DC_OVER_VOLTAGE,
	DC_CURRENT_LEAKS,
	I2C_OK,
	I2C_ERROR,
	PWM_OK,
	PWM_ERROR,
	BOARD_CONFIG_OK,
	BOARD_CONFIG_ERROR
};

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
