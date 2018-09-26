#ifndef _NS_CONSTS_H
#define _NS_CONSTS_H

#include "mbed.h"

static const float NECTAR_POWER_BOARD_VERSION = 108;

// Voltage & Current Sensor I/O
static const PinName SDA = PB_14;
static const PinName SCL = PB_13;
static const uint8_t V_SENSE_ADDR = 0x55 << 1;
static const uint8_t I_SENSE_ADDR = 0x5A << 1;
static const float V_REF = 3.00;
static const float INPUT_VDIV = 4.1/400;
static const float CURRENT_LIMIT = 10.0;
static const float VOLTAGE_LIMIT = 385.0;
// static const float VOLTAGE_LIMIT = 440.0; // vido config

// PWM Output consts
static const PinName PWM_PIN = PB_1;
static const PinName SD_PIN = PB_0;
static const bool DRIVER_ON = false;
static const bool DRIVER_OFF = true;
static const float CURRENT_THRESHOLD = 0.1;
static const float PWM_FREQUENCY = 1.8;
static const float PWM_DUTY_MIN = 0.1;
static const float PWM_DUTY_MAX = 0.95;
static const float PWM_STEP = 0.02;
static const float PWM_STEP_FINE = 0.005;

// Temperature Limits
static const int PROCESSOR_INTERNAL_TEMPERATURE_LIMIT = 120;

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
static const bool RESET_ENERGY_METERS = false;

#endif
