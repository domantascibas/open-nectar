#ifndef CONSTS_H
#define CONSTS_H

#include "mbed.h"

static const float NECTAR_POWER_BOARD_VERSION = 107;

// PC Serial comms
static const uint32_t PC_BAUD = 115200;
static const PinName PC_TX = PA_2;
static const PinName PC_RX = PA_3;

// Inter-board Serial comms
static const PinName NS_TX = PC_10;
static const PinName NS_RX = PC_11;

// Other I/O
static const PinName USER_LED = PA_15;
static const PinName OVERHEAT = PD_2;
static const PinName CALIBRATION_BTN = PC_12;
static const PinName LOW_VOLTAGE_MONITOR = PC_9;

// Sensor consts
static const PinName SDA = PB_14;
static const PinName SCL = PB_13;
static const float V_REF = 3.00;

// PWM Output consts
static const PinName PWM_PIN = PB_1;
static const PinName SD_PIN = PB_0;
static const float PWM_STEP = 0.02;
static const float CURRENT_THRESHOLD = 0.1;

// Limits
static const float VOLTAGE_LIMIT = 385.0;
//const float VOLTAGE_LIMIT = 25.0;
// const float VOLTAGE_LIMIT = 440.0; // vido config
static const float CURRENT_LIMIT = 10.0;
//const float LEAKAGE_CURRENT = 0.4;

//static const uint8_t FILTER_LENGTH = 5;
static const uint8_t V_SENSE_ADDR = 0x55 << 1;
static const uint8_t I_SENSE_ADDR = 0x5A << 1;
static const float INPUT_VDIV = 4.1/400;
static const int PROCESSOR_INTERNAL_TEMPERATURE_LIMIT = 120;
//static const PinName DEVICE_TEMP_PROBE = PC_7;
//static const float DEVICE_TEMPERATURE_LIMIT_MAX = 85.0;

static const bool RESET_ENERGY_METERS = false;

#endif
