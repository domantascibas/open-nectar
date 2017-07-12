#ifndef COMMS_H
#define COMMS_H

#define POWER_BOARD_START       0x0F
#define POWER_BOARD_STOP        0xF0
#define RECEIVE_VOLTAGE         0x56    //'V'
#define RECEIVE_CURRENT         0x49    //'I'

#define SEND_PWM_DUTY           0x44    //'D'
#define SEND_SHUTDOWN           0x53    //'S'

namespace power_board {
    uint8_t start(void);
    uint8_t stop(void);
    
    double get_voltage(void);
    double get_current(void);
}

namespace power_board_tests {
    uint8_t pwm(float);
    uint8_t shutdown(void);
}

namespace esp {
    uint8_t read_message(void);
    
    //cmd from ESP
    //temperature_min
    //temperature_max
    //temperature_scheduled
    //current_mode
    //cancel_boost
    
    //return to ESP
    //moment_sun_watt
    //pwm_duty
    //sun_voltage
    //sun_current
    //sun_relay_on
    //grid_relay_on
    //water_temperature
    //device_temperature
    //capacitor_temperature
    //transistor_overheat_on
}

#endif

// *******************************Nectar Sun Copyright � Nectar Sun 2017*************************************   
