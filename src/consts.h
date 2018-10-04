#ifndef _NS_CONSTS_H
#define _NS_CONSTS_H

#include "mbed.h"

static const char NECTAR_DEVICE_MODEL[] = "Nectar Sun S";
static const float NECTAR_MAIN_BOARD_VERSION = 109;

// Temperature
static const int PROCESSOR_INTERNAL_TEMPERATURE_LIMIT = 120;
static const float BOOST_TEMP = 70.0;
static const float AWAY_TEMP = 10.0;
static const float HIST = 2.0;

// Timing
static const float POWER_INIT_TIMEOUT = 5;
static const float ESP_INIT_TIMEOUT = 20;
// Boost timeout. Turns off boost mode in case it can't reach 70C in certain amount of time
static const uint8_t BOOST_TIMEOUT = 180;
static const float ENERGY_METER_UPDATE_INTERVAL = 5.0;
static const uint16_t LEGIONELLA_PREVENTION_PERIOD = 5;	//in days

// Pins
static const PinName ZERO_CROSS_PIN = PB_5;
static const PinName GRID_RELAY_PIN = PB_4;
static const PinName SUN_RELAY_PIN = PB_3;
static const PinName BOILER_TEMP_PIN = PB_8;

static const float WATER_TEMPERATURE_LIMIT_MIN = 5.0;
static const float WATER_TEMPERATURE_LIMIT_MAX = 90.0;

// Storage
static const uint8_t DEVICE_CONFIGURED = 0xC0;
static const uint8_t ESP_CONFIGURED = 0xEC;
static const uint8_t EMPTY_VALUE = 0xFA;

// Power board comms
static const PinName POWER_COMMS_TX_PIN = PB_10;
static const PinName POWER_COMMS_RX_PIN = PB_11;
static const float POWER_COMMS_PING_INTERVAL = 2.0;

// ESP comms
static const PinName ESP_COMMS_TX_PIN = PA_9;   //D8
static const PinName ESP_COMMS_RX_PIN = PA_10;  //D2
static const float ESP_COMMS_PING_INTERVAL = 15.0;

// Service Comms
static const PinName SERVICE_COMMS_TX_PIN = PA_2;
static const PinName SERVICE_COMMS_RX_PIN = PA_3;
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
static const PinName MENU_BUTTON_BACKLIGHT_PIN = PD_2;
static const PinName MENU_BUTTON_UP_PIN = PA_5;
static const PinName MENU_BUTTON_DOWN_PIN = PA_7;
static const PinName MENU_BUTTON_CENTER_PIN = PA_6;

static const bool MENU_BUTTON_BACKLIGHT_ON = true;
static const bool MENU_BUTTON_BACKLIGHT_OFF = false;
static const float MENU_BUTTON_DEBOUNCE_PERIOD = 0.2;			// in seconds
static const float MENU_BUTTON_LONG_PRESS_PERIOD = 2.0;		// in seconds
static const float MENU_BUTTON_ENTER_PRESS_PERIOD = 0.4;	// in seconds
static const float MENU_TIME_KEEPER_PERIOD = 60.0;				// in seconds
static const float MENU_TIMEOUT_TO_IDLE = 60.0;						// in seconds
static const uint8_t MENU_PAIRING_PERIOD = 30;						// in minutes
static const uint8_t MENU_SCREEN_INIT_PERIOD = 10;				// in minutes

static const uint64_t MENU_DEFAULT_INIT_TIME = 1513080000;	// sets time to 12/12/2017 @ 12:00:00 UTC

// Screen
static const PinName SCREEN_SDA_PIN = PB_7;
static const PinName SCREEN_SCL_PIN = PB_6;
static const uint32_t SCREEN_I2C_FREQUENCY = 400000;

#endif
