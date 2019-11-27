#ifndef _NS_RELEASE_SETTINGS_H
#define _NS_RELEASE_SETTINGS_H

#include "mbed.h"

static const float REL_VOLTAGE_LIMIT = 385.0;
static const float REL_CURRENT_LIMIT = 10.0;

static const float REL_CALIBRATION_VOLTAGE_MAX = 0.04;
static const float REL_CALIBRATION_VOLTAGE_MIN = 0.001;
static const float REL_CALIBRATION_CURRENT_MAX = 2.2;
static const float REL_CALIBRATION_CURRENT_MIN = 1.8;

static const int REL_DEVICE_TEMPERATURE_LIMIT_MAX = 85.0;
static const int REL_PROCESSOR_INTERNAL_TEMPERATURE_LIMIT = 120;

static const float REL_PWM_FREQUENCY = 1.8;
static const float REL_PWM_DUTY_MAX = 0.95;
static const float REL_PWM_DUTY_MIN = 0.1;
static const float REL_PWM_STEP = 0.02;
static const float REL_PWM_STEP_FINE = 0.005;
static const float REL_CURRENT_THRESHOLD = 0.1;

static const uint32_t REL_PC_BAUD = 115200;

#endif