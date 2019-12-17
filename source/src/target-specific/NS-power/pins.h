#ifndef _NS_PINS_H
#define _NS_PINS_H

#include "mbed.h"

#define I2C_SDA_PIN PB_14
#define I2C_SCL_PIN PB_13

#define PWM_PIN PB_1
#define SD_PIN PB_0

#define INTERNAL_TEMPERATURE_PIN PC_7

#define PC_TX PA_2
#define PC_RX PA_3

#define NS_TX PC_10
#define NS_RX PC_11

#define USER_LED PA_15
#define OVERHEAT PD_2
#define CALIBRATION_BTN PC_12
#define LOW_VOLTAGE_MONITOR PC_9

#endif
