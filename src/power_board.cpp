#include "mbed.h"
#include "data.h"
#include "comms.h"

extern RawSerial comms_pc;
extern RawSerial comms_power;

namespace power_board {
    uint8_t get_data(void);
    //char buffer[128];

    void init(uint32_t baudrate) {
        comms_power.baud(baudrate);
    }
    
    uint8_t loop(void) {
        comms_pc.printf("POWER BOARD Getting data\r\n");
        //print what get_data() returns
        return get_data();
    }
    
    uint8_t start(void) {
        return comms::send_cmd_power(CMD_POWER_BOARD_START);
    }
    
    uint8_t stop(void) {
        return comms::send_cmd_power(CMD_POWER_BOARD_STOP);
    }
    
    uint8_t get_data(void) {
        uint8_t response = comms::send_cmd_power(KEYBOARD_GET_DATA);
        if(response == INCOMING_DATA) {
            comms_pc.printf("INCOMING DATA\r\n");
            response = comms::receive_message_power();
        } else {
            comms_pc.printf("[ERROR] %d\r\n", response);
            while(comms_power.readable()) {
                comms_power.getc();
            }
        }
        return response;
    }
    
    uint8_t get_calibration_data(void) {
        uint8_t response = comms::send_cmd_power(KEYBOARD_GET_CALIB_DATA);
        if(response == INCOMING_DATA) {
            response = comms::receive_message_power();
        } else {
            comms_pc.printf("[ERROR] %d\r\n", response);
            while(comms_power.readable()) {
                comms_power.getc();
            }
        }
        return response;
    }

    uint8_t shutdown(bool status_off) {
        if(status_off) {
            //DRIVER_OFF
            return comms::send_cmd_power(CMD_PWM_OFF);
        } else {
            //DRIVER_ON
            return comms::send_cmd_power(CMD_PWM_ON);
        }
    }
    
    uint8_t enter_service_menu(bool service_menu) {
        uint8_t response;
        if(service_menu) {
            response = comms::send_cmd_power(MANUAL_MODE);
            if(response == ACK) {
                response = comms::send_cmd_power(CMD_GET_PWM_DUTY);
                if(response == INCOMING_DATA) {
                    while(!comms_power.readable()) {}
                    //comms_power.scanf("#%f$", &data.pwm_duty);
                    comms_power.getc();
                    return NS_OK;
                }else {
                    return response;
                }
            } else {
                return response;
            }
        } else {
            return comms::send_cmd_power(AUTO_MODE);
        }
    }
    
    uint8_t increase_pwm(bool increase) {
        if(data.pwm_duty < 0.1) {
            data.pwm_duty = 0.1;
        }
        uint8_t response = comms::send_cmd_power(CMD_SET_PWM_DUTY);
        if(response == WAITING_FOR_DATA) {
            while(!comms_power.writeable()) {}
            if(increase) {
                if((data.pwm_duty + 0.1) > 0.95) {
                    data.pwm_duty = 0.95;
                } else {
                    data.pwm_duty += 0.1;
                }
            } else {
                if((data.pwm_duty - 0.1) < 0.1) {
                    data.pwm_duty = 0.1;
                } else {
                    data.pwm_duty -= 0.1;
                }
            }
            comms_power.printf("#%f$\n", data.pwm_duty);
            comms_pc.printf("set PWM duty %f\r\n", data.pwm_duty);
            return NS_OK;
        } else {
            comms_pc.printf("Error: 0x%X\r\n", response);
        }
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
