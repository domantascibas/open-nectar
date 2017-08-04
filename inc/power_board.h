#ifndef POWER_BOARD_H
#define POWER_BOARD_H

namespace power_board {
    void init(uint32_t);
    uint8_t loop(void);
    uint8_t start(void);
    uint8_t stop(void);
    uint8_t get_data(void);
    uint8_t get_calibration_data(void);
    
    //toggles the PWM generator
    //shutdown(DRIVER_ON(=0)) turns the PWM driver ON
    //shutdown(DRIVER_OFF(=1)) turns the PWM driver OFF
    uint8_t shutdown(bool);
    uint8_t enter_service_menu(bool);
    uint8_t increase_pwm(bool);
}

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
