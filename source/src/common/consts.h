#ifndef _NS_CONSTS_H
#define _NS_CONSTS_H

#include "mbed.h"
#include <stdint.h>

#define NECTAR_DEVICE_MODEL "Nectar Sun S"
#define NECTAR_MAIN_BOARD_VERSION 110
#define NECTAR_POWER_BOARD_VERSION NECTAR_MAIN_BOARD_VERSION


static const float DEVICE_MODE_UPDATE_INTERVAL = 0.5f;

/* POWER */
// Calibration settings
static const float CALIBRATION_VOLTAGE_MAX = 0.04f;
static const float CALIBRATION_VOLTAGE_MIN = 0.001f;
static const float CALIBRATION_CURRENT_MAX = 2.2f;
static const float CALIBRATION_CURRENT_MIN = 1.8f;

// Voltage & Current Sensor I/O
static const uint8_t V_SENSE_ADDR = 0x55 << 1;
static const uint8_t I_SENSE_ADDR = 0x5A << 1;
static const uint32_t I2C_FREQUENCY = 400000;
static const float V_REF = 3.00f;
static const float INPUT_VDIV = 4.1f / 400;
static const float CURRENT_LIMIT = 10;
static const float VOLTAGE_LIMIT = 385;
// static const float VOLTAGE_LIMIT = 440.0; // vido config

// PWM Output consts
static const bool DRIVER_ON = false;
static const bool DRIVER_OFF = true;
static const float CURRENT_THRESHOLD = 0.1f;
static const float PWM_FREQUENCY = 1.8f;
static const float PWM_DUTY_MIN = 0.1f;
static const float PWM_DUTY_MAX = 0.95f;
static const float PWM_STEP = 0.02f;
static const float PWM_STEP_FINE = 0.005f;

// Temperature Limits
static const float DEVICE_TEMPERATURE_LIMIT_MAX = 85;
static const int PROCESSOR_INTERNAL_TEMPERATURE_LIMIT = 120;

// PC Serial comms
static const uint32_t PC_BAUD = 115200;

// Inter-board Serial comms

// Other I/O


/* MAIN */
// Temperature
static const float BOOST_TEMP = 70.0;
static const float AWAY_TEMP = 10.0;
static const float HIST = 2.0;
static const uint8_t LOST_SENSOR_COUNT = 100;
static const uint8_t TEMPERATURE_SENSOR_UPDATE_PERIOD = 3;

// Timing
static const float POWER_INIT_TIMEOUT = 5;
static const float ESP_INIT_TIMEOUT = 20;
// Boost timeout. Turns off boost mode in case it can't reach 70C in certain amount of time
static const uint8_t BOOST_TIMEOUT = 180;
static const float ENERGY_METER_UPDATE_INTERVAL = 5.0;
static const uint16_t LEGIONELLA_PREVENTION_PERIOD = 5; //in days

// Pins

static const float WATER_TEMPERATURE_LIMIT_MIN = 5.0;
static const float WATER_TEMPERATURE_LIMIT_MAX = 90.0;

// Storage
static const uint8_t DEVICE_CONFIGURED = 0xC0;
static const uint8_t ESP_CONFIGURED = 0xEC;
static const uint8_t EMPTY_VALUE = 0xFA;

// Power board comms
static const float POWER_COMMS_PING_INTERVAL = 1.0;

// ESP comms
static const float ESP_COMMS_PING_INTERVAL = 15.0;

// Service Comms
static const uint32_t SERVICE_COMMS_BAUD_RATE = 115200;
static const float TEST_MODE_GRID_ON_TEMPERATURE = 30.0;
static const float TEST_MODE_SUN_ON_TEMPERATURE = 60.0;
static const float TEST_MODE_IDLE_ON_TEMPERATURE = 80.0;

static const uint8_t TEST_MODE_TURN_ON_GRID_CMD = 0x67;
static const uint8_t TEST_MODE_TURN_ON_SUN_CMD = 0x73;
static const uint8_t TEST_MODE_TURN_ON_IDLE_CMD = 0x69;
static const uint8_t TEST_MODE_ENTER_TEST_MODE_CMD = 0x74;

// Errors
static const uint32_t MAX_ERROR_CODE = 16383;

// Menu Service

static const bool MENU_BUTTON_BACKLIGHT_ON = true;
static const bool MENU_BUTTON_BACKLIGHT_OFF = false;
static const float MENU_BUTTON_DEBOUNCE_PERIOD = 0.2;           // in seconds
static const float MENU_BUTTON_LONG_PRESS_PERIOD = 2.0;     // in seconds
static const float MENU_BUTTON_ENTER_PRESS_PERIOD = 0.4;    // in seconds
static const float MENU_TIME_KEEPER_PERIOD = 60.0;              // in seconds
static const float MENU_TIMEOUT_TO_IDLE = 60.0;                     // in seconds
static const uint8_t MENU_PAIRING_PERIOD = 30;                      // in minutes
static const uint8_t MENU_SCREEN_INIT_PERIOD = 10;              // in minutes

static const uint64_t MENU_DEFAULT_INIT_TIME = 1513080000;  // sets time to 12/12/2017 @ 12:00:00 UTC

// Screen
static const uint32_t SCREEN_I2C_FREQUENCY = 400000;

#endif
