#ifndef _NS_PINS_H
#define _NS_PINS_H

#include "mbed.h"

#define ZERO_CROSS_PIN PB_5
#define GRID_RELAY_PIN PB_4
#define SUN_RELAY_PIN PB_3
#define BOILER_TEMP_PIN PB_8

#define POWER_COMMS_TX_PIN PB_10
#define POWER_COMMS_RX_PIN PB_11

#define ESP_COMMS_TX_PIN PA_9   //D8    connected to ESP pin5
#define ESP_COMMS_RX_PIN PA_10  //D2    connected to ESP pin4

#define SERVICE_COMMS_TX_PIN PA_2
#define SERVICE_COMMS_RX_PIN PA_3

#define MENU_BUTTON_BACKLIGHT_PIN PD_2
#define MENU_BUTTON_UP_PIN PA_5
#define MENU_BUTTON_DOWN_PIN PA_7
#define MENU_BUTTON_CENTER_PIN PA_6

#define SCREEN_SDA_PIN PB_7
#define SCREEN_SCL_PIN PB_6

#define U1WIRE_PIN BOILER_TEMP_PIN

#endif
