#ifndef COMMS_H
#define COMMS_H

//Commands
#define KEYBOARD_STOP               0x30    //'0'
#define KEYBOARD_START              0x31    //'1'
#define KEYBOARD_GET_DATA           0x32    //'2'
#define KEYBOARD_GET_CALIB_DATA     0x33    //'3'
#define KEYBOARD_PWM_OFF            0x38    //'8'
#define KEYBOARD_PWM_ON             0x39    //'9'
#define INCREASE_PWM_DUTY           0x2B    //'+'
#define DECREASE_PWM_DUTY           0x2D    //'-'
#define AUTO_MODE                   0x41    //'A'
#define MANUAL_MODE                 0x4D    //'M'

#define CMD_POWER_BOARD_START       0x0F
#define CMD_POWER_BOARD_STOP        0xF0

#define CMD_GET_VOLTAGE             0x56    //'V'
#define CMD_GET_CURRENT             0x49    //'I'
#define CMD_GET_REF_VOLTAGE         0x4B    //'K'
#define CMD_GET_REF_CURRENT         0x4C    //'L'

#define CMD_SET_PWM_DUTY            0x44    //'D'
#define CMD_GET_PWM_DUTY            0x45    //'E'
#define CMD_GET_CAP_TEMP            0x4E    //'P'
#define CMD_GET_MOSFET_OHEAT        0x4F    //'O'

#define CMD_PWM_ON                  0x41    //'A'
#define CMD_PWM_OFF                 0x42    //'B'

#define SET_MODE                    0x4D    //'M'
#define GET_STATS                   0x54    //'T'

//Constants
#define DRIVER_ON                   0
#define DRIVER_OFF                  1
#define ACK                         0x06
#define NACK                        0x15
#define INCOMING_DATA               0x24    //'$'
#define WAITING_FOR_DATA            0x23    //'#'
#define BAD_CMD                     0x21    //'!'

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
