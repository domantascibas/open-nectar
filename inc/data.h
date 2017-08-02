#ifndef DATA_H
#define DATA_H

#define PWM_MIN					0.1
#define PWM_MAX					0.95
#define	V_REF					3.00

#define OVERVOLTAGE				25.0
#define OVERCURRENT				10.0

#define LED_ON                  0
#define LED_OFF                 1

#define DRIVER_ON               0
#define DRIVER_OFF              1

enum error_codes{
	SETUP_ERROR,
	STARTUP_ERROR,
	ADC_ERROR,
    ADC_VOLTAGE_ERROR,
    ADC_CURRENT_ERROR,
	ADC_SETUP_ERROR,
	FLASH_ACCESS_ERROR,
	FLASH_READ_ERROR,
	FLASH_WRITE_ERROR,
	CALIBRATION_ERROR,
	DC_OVER_VOLTAGE,
    DC_OVER_CURRENT,
	DC_CURRENT_LEAKS,
	I2C_ERROR,
    PWM_OK,
	PWM_ERROR,
	BOARD_CONFIG_ERROR,
    OVERHEAT,
    RADIATOR_OVERHEAT,
    AIRGAP_OVERHEAT
};

enum codes {
    NS_OK,
    NS_ERROR,
    MSG_OK,
    MSG_ERROR
};

struct Data {
    float moment_voltage;
    float moment_current;
    float moment_power;
    
    float reference_voltage;
    float reference_current;
    
    float pwm_duty;
    float airgap_temperature;
    float radiator_temperature;
    
    bool mosfet_overheat_on;
    bool calibrated;
};

extern Data data;

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
