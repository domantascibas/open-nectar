#ifndef COMMS_H
#define COMMS_H

namespace pc_monitor {
    void init(uint32_t);    
    void loop(void);
    
    //commands to manually control the power board
}

namespace esp {
    void init(uint32_t);
    
    //reads message from ESP
    //sets the temperature_max, temperature_min, temperature_scheduled and current_mode
    uint8_t set_mode(void);
    
    //sends data back to ESP as a string
    //  temperature_moment,
    //  sun_power_moment,
    //  sun_voltage,
    //  sun_current,
    //  pwm_duty,
    //  device_temperature,
    //  capacitor_temperature,
    //  sun_relay_on,
    //  grid_relay_on,
    //  transistor_overheat_on
    uint8_t get_stats(void);
    
    uint8_t cancel_boost(void);
}

namespace power_board {
    void init(uint32_t);
    
    //starts the power board
    uint8_t start(void);
    
    //stops the power board
    uint8_t stop(void);
    
    //get voltage, current, pwm_duty, Tcapacitor, and mosfet_overheat_on
    uint8_t get_data(void);
    
    //get ref_voltage and ref_current
    uint8_t get_calibration_data(void);
    
    //toggles the PWM generator
    //shutdown(DRIVER_ON(=0)) turns the PWM driver ON
    //shutdown(DRIVER_OFF(=1)) turns the PWM driver OFF
    uint8_t shutdown(bool);
    
    uint8_t enter_service_menu(bool);
    
    uint8_t increase_pwm(bool);
}

namespace power_board_tests {
    uint8_t set_pwm(float);
    uint8_t shutdown(bool);
}

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
