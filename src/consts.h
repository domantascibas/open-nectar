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

#endif
