#include "mbed.h"
#include "utils.h"
#include "pin_names.h"
#include "comms.h"
#include "data.h"

Serial comms_power(COMMS_POWER_TX, COMMS_POWER_RX);
Serial comms_pc(COMMS_PC_TX, COMMS_PC_RX);

namespace power_board {
    uint8_t start(void) {
        comms_power.putc(POWER_BOARD_START);
        return 0;
    }
    
    uint8_t stop(void) {
        comms_power.putc(POWER_BOARD_STOP);
        return 0;
    }
    
    double get_voltage(void) {
        float voltage;
        //int result;
        comms_power.putc(RECEIVE_VOLTAGE);
        while(!comms_power.readable()) {
        }
        //result = comms_power.scanf("%f", &voltage);
        comms_power.scanf("%f", &voltage);
        comms_power.getc();
        return voltage;
    }

    double get_current(void) {
        float current;
        comms_power.putc(RECEIVE_CURRENT);
        while(!comms_power.readable()) {
        }
        comms_power.scanf("%f", &current);
        comms_power.getc();
        return current;
    }
}

namespace power_board_tests {
    uint8_t pwm(float duty) {
        comms_power.putc(SEND_PWM_DUTY);
        comms_power.printf("%f\n", duty);
        
        return 0;
    }

    uint8_t shutdown(void) {
        comms_power.putc(SEND_SHUTDOWN);
        return 0;
    }
}

namespace esp {
    uint8_t read_message(void) {
        //read message from ESP
        data.temperature_min = 18.0;
        data.temperature_max = 80.0;
        data.temperature_scheduled = 56.0;
        data.current_mode = MODE_DEFAULT;
        return 0;
    }
    
    uint8_t set_data() {
        
        return 0;
    }
    
    uint8_t cancel_boost() {
        return 0;
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
