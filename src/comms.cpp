#include "mbed.h"
#include "utils.h"
#include "pin_names.h"
#include "comms.h"
#include "data.h"

Serial comms_power(COMMS_POWER_TX, COMMS_POWER_RX);
Serial comms_pc(COMMS_PC_TX, COMMS_PC_RX);

extern data_collection nectar_data;

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
    
    uint8_t shutdown(bool status_off) {
        if(status_off) {
            comms_power.putc(SHUTDOWN_STATUS_OFF);
        } else {
            comms_power.putc(SHUTDOWN_STATUS_ON);
        }
        return 0;
    }
}

namespace power_board_tests {
    uint8_t pwm(float duty) {
        comms_power.putc(SEND_PWM_DUTY);
        comms_power.printf("%f\n", duty);
        
        return 0;
    }

    uint8_t shutdown(bool status_off) {
        if(status_off) {
            comms_power.putc(SHUTDOWN_STATUS_OFF);
        } else {
            comms_power.putc(SHUTDOWN_STATUS_ON);
        }
        return 0;
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
