#include "mbed.h"
#include "utils.h"
#include "pin_names.h"
#include "comms.h"

Serial comms_power(COMMS_POWER_TX, COMMS_POWER_RX);
Serial comms_pc(COMMS_PC_TX, COMMS_PC_RX);

uint8_t power_board::start(void) {
    comms_power.putc(POWER_BOARD_START);
    return 0;
}

uint8_t power_board::stop(void) {
    comms_power.putc(POWER_BOARD_STOP);
    return 0;
}

double power_board::get_voltage(void) {
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

double power_board::get_current(void) {
    float current;
    comms_power.putc(RECEIVE_CURRENT);
    while(!comms_power.readable()) {
    }
    comms_power.scanf("%f", &current);
    comms_power.getc();
    return current;
}

uint8_t power_board_tests::pwm(float duty) {
    comms_power.putc(SEND_PWM_DUTY);
    comms_power.printf("%f\n", duty);
    
    return 0;
}

uint8_t power_board_tests::shutdown(void) {
    comms_power.putc(SEND_SHUTDOWN);
    return 0;
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
