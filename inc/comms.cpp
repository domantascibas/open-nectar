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
    double voltage;
    char received;
    comms_power.putc(RECEIVE_VOLTAGE);
    while(!comms_power.readable()) {
    }
    while(comms_power.readable()) {
        received = comms_power.getc();
        printf("Received1: %c", received);
    }
    return 0.23;
}

double power_board::get_current(void) {
    
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
