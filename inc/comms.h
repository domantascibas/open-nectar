#ifndef COMMS_H
#define COMMS_H

#define POWER_BOARD_START       0x0F
#define POWER_BOARD_STOP        0xF0
#define RECEIVE_VOLTAGE         0x56    //'V'
#define RECEIVE_CURRENT         0x49    //'I'

#define SEND_PWM_DUTY           0x44    //'D'
#define SHUTDOWN_STATUS_ON      0x41    //'A'
#define SHUTDOWN_STATUS_OFF     0x42    //'B'

#define COMMS_POWER_TX      PB_10
#define COMMS_POWER_RX      PB_11

namespace power_board {
    uint8_t start(void);
    uint8_t stop(void);
    
    double get_voltage(void);
    double get_current(void);
    
    uint8_t shutdown(bool);
}

namespace power_board_tests {
    uint8_t pwm(float);
    uint8_t shutdown(bool);
}

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
