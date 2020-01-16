#ifndef _NS_CONSTS_H
#define _NS_CONSTS_H

#include "mbed.h"
#include <stdint.h>

#define NECTAR_DEVICE_MODEL "Nectar Sun S"
#define NECTAR_MAIN_BOARD_VERSION 110
#define NECTAR_POWER_BOARD_VERSION NECTAR_MAIN_BOARD_VERSION


// Calibration settings
#define CALIBRATION_VOLTAGE_MAX 0.04
#define CALIBRATION_VOLTAGE_MIN 0.001
#define CALIBRATION_CURRENT_MAX 2.2
#define CALIBRATION_CURRENT_MIN 1.8

// Voltage & Current Sensor I/O
#define V_SENSE_ADDR 0x55 << 1
#define I_SENSE_ADDR 0x5A << 1
#define I2C_FREQUENCY 400000
#define SCREEN_I2C_FREQUENCY I2C_FREQUENCY
#define V_REF 3.00
#define INPUT_VDIV 4.1f / 400
#define CURRENT_LIMIT 10
#define VOLTAGE_LIMIT 385
// #define VOLTAGE_LIMIT 440.0 // vido config

// PWM Output consts
#define DRIVER_ON 0
#define DRIVER_OFF 1
#define CURRENT_THRESHOLD 0.1
#define PWM_FREQUENCY 1.8
#define PWM_DUTY_MIN 0.1
#define PWM_DUTY_MAX 0.95
#define PWM_STEP 0.02
#define PWM_STEP_FINE 0.005

// Temperature Limits
#define DEVICE_TEMPERATURE_LIMIT_MAX 85
#define PROCESSOR_INTERNAL_TEMPERATURE_LIMIT 120

#define BOOST_TEMP 70.0
#define AWAY_TEMP 10.0
#define HIST 2.0
#define LOST_SENSOR_COUNT 255

#define WATER_TEMPERATURE_LIMIT_MIN 5.0
#define WATER_TEMPERATURE_LIMIT_MAX 90.0

// PC Serial comms
#define PC_BAUD 115200
#define SERVICE_COMMS_BAUD_RATE PC_BAUD

// Timing
#define POWER_INIT_TIMEOUT 5
#define POWER_COMMS_PING_INTERVAL 1.0
#define ESP_INIT_TIMEOUT 20
#define ESP_COMMS_PING_INTERVAL 15.0
// Boost timeout. Turns off boost mode in case it can't reach 70C in certain amount of time
#define BOOST_TIMEOUT 180

#define TEMPERATURE_SENSOR_UPDATE_PERIOD 3
#define LEGIONELLA_PREVENTION_PERIOD 5 //in days
#define ENERGY_METER_UPDATE_INTERVAL 5.0
#define DEVICE_MODE_UPDATE_INTERVAL 0.5

// Storage
#define DEVICE_CONFIGURED 0xC0
#define ESP_CONFIGURED 0xEC
#define EMPTY_VALUE 0xFA

// Service Comms
#define TEST_MODE_GRID_ON_TEMPERATURE 30.0
#define TEST_MODE_SUN_ON_TEMPERATURE 60.0
#define TEST_MODE_IDLE_ON_TEMPERATURE 80.0

#define TEST_MODE_TURN_ON_GRID_CMD 0x67
#define TEST_MODE_TURN_ON_SUN_CMD 0x73
#define TEST_MODE_TURN_ON_IDLE_CMD 0x69
#define TEST_MODE_ENTER_TEST_MODE_CMD 0x74

// Errors
#define MAX_ERROR_CODE 16383

// Menu Service
#define MENU_BUTTON_BACKLIGHT_ON 1
#define MENU_BUTTON_BACKLIGHT_OFF 0
#define MENU_BUTTON_DEBOUNCE_PERIOD 0.2           // in seconds
#define MENU_BUTTON_LONG_PRESS_PERIOD 2.0     // in seconds
#define MENU_BUTTON_ENTER_PRESS_PERIOD 0.4    // in seconds
#define MENU_TIME_KEEPER_PERIOD 60.0              // in seconds
#define MENU_TIMEOUT_TO_IDLE 60.0                     // in seconds
#define MENU_PAIRING_PERIOD 30                      // in minutes
#define MENU_SCREEN_INIT_PERIOD 10              // in minutes

#define MENU_DEFAULT_INIT_TIME 1513080000  // sets time to 12/12/2017 @ 12:00:00 UTC

#endif
