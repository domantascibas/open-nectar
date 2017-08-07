#include "mbed.h"
#include "data.h"
#include "power_board.h"
#include "comms.h"

static const uint8_t CMD_POWER_BOARD_START = 0x0F;
static const uint8_t CMD_POWER_BOARD_STOP = 0xF0;

static const uint8_t CMD_GET_REF_DATA = 0x43;       //'C'
#define KEYBOARD_GET_DATA           0x32    //'2'
static const uint8_t CMD_GET_DATA = 0x44;           //'D'

static const uint8_t CMD_PWM_ON = 0x41;             //'A'
static const uint8_t CMD_PWM_OFF = 0x42;            //'B'

static const PinName TX = PB_10;
static const PinName RX = PB_11;

RawSerial power_board_serial(TX, RX);
extern RawSerial pc;
Ticker get_data_tick;

namespace power_board {
    char* power_response[NUM_FIELDS];
    char received_chars[128];
    
    volatile bool new_data = false;
    volatile bool recv_in_progress = false;
    volatile uint8_t ndx = 0;
    volatile char rc;
    const uint8_t num_chars = 128;
    const char start_marker = '#';
    const char end_marker = '$';
        
    void Rx_interrupt() {
        rc = power_board_serial.getc();
        
        if(recv_in_progress) {
            if(rc != end_marker) {
                received_chars[ndx] = rc;
                ndx++;
                pc.putc(rc);
            } else {
                while(power_board_serial.readable()) {
                    pc.putc(power_board_serial.getc());
                }
                recv_in_progress = false;
                ndx = 0;
                new_data = true;
            }
        } else if(rc == start_marker){
            recv_in_progress = true;
        }
    }
    
    void start() {
        comms::send_cmd_power(CMD_POWER_BOARD_START);
    }
    
    void stop() {
        comms::send_cmd_power(CMD_POWER_BOARD_STOP);
    }
    
    void get_data() {
        uint8_t response = comms::send_cmd_power(KEYBOARD_GET_DATA);
    }
    
    uint8_t get_calibration_data(void) {
//        uint8_t response = comms::send_cmd_power(KEYBOARD_GET_CALIB_DATA);
//        if(response == INCOMING_DATA) {
//            response = comms::receive_message_power();
//        } else {
//            comms_pc.printf("[ERROR] %d\r\n", response);
//            while(comms_power.readable()) {
//                comms_power.getc();
//            }
//        }
//        return response;
    }

    uint8_t shutdown(bool status_off) {
//        if(status_off) {
//            //DRIVER_OFF
//            return comms::send_cmd_power(CMD_PWM_OFF);
//        } else {
//            //DRIVER_ON
//            return comms::send_cmd_power(CMD_PWM_ON);
//        }
    }
    
    uint8_t enter_service_menu(bool service_menu) {
//        uint8_t response;
//        if(service_menu) {
//            response = comms::send_cmd_power(MANUAL_MODE);
//            if(response == ACK) {
//                response = comms::send_cmd_power(CMD_GET_PWM_DUTY);
//                if(response == INCOMING_DATA) {
//                    while(!comms_power.readable()) {}
//                    //comms_power.scanf("#%f$", &data.pwm_duty);
//                    comms_power.getc();
//                    return NS_OK;
//                }else {
//                    return response;
//                }
//            } else {
//                return response;
//            }
//        } else {
//            return comms::send_cmd_power(AUTO_MODE);
//        }
    }
    
    uint8_t increase_pwm(bool increase) {
//        if(data.pwm_duty < 0.1) {
//            data.pwm_duty = 0.1;
//        }
//        uint8_t response = comms::send_cmd_power(CMD_SET_PWM_DUTY);
//        if(response == WAITING_FOR_DATA) {
//            while(!comms_power.writeable()) {}
//            if(increase) {
//                if((data.pwm_duty + 0.1) > 0.95) {
//                    data.pwm_duty = 0.95;
//                } else {
//                    data.pwm_duty += 0.1;
//                }
//            } else {
//                if((data.pwm_duty - 0.1) < 0.1) {
//                    data.pwm_duty = 0.1;
//                } else {
//                    data.pwm_duty -= 0.1;
//                }
//            }
//            comms_power.printf("#%f$\n", data.pwm_duty);
//            comms_pc.printf("set PWM duty %f\r\n", data.pwm_duty);
//            return NS_OK;
//        } else {
//            comms_pc.printf("Error: 0x%X\r\n", response);
//        }
    }
    
    void get_data_ISR() {
        comms::send_cmd_power(KEYBOARD_GET_DATA);
    }
    
    void setup() {
        power_board_serial.baud(19200);
        power_board::start();
        get_data_tick.attach(&get_data_ISR, 2.0);
        power_board_serial.attach(&Rx_interrupt);
    }
    
    void loop() {
        uint8_t message_count;
        
        if(new_data) {
            new_data = false;
            pc.puts(" [PARSING] ");
            message_count = comms::parse_fields(received_chars, power_response, NUM_FIELDS, ',');
            if(message_count == 6) {
                __disable_irq();
                data.pv_voltage = atof(power_response[0]);
                data.pv_current = atof(power_response[1]);
                data.pwm_duty = atof(power_response[2]);
                data.radiator_temp = atof(power_response[3]);
                data.mosfet_overheat_on = atoi(power_response[4]);
                data.airgap_temp = atof(power_response[5]);

                data.pv_power = data.pv_voltage * data.pv_current;
                __enable_irq();
                pc.puts("parsed\r\n");
            } else {
                pc.puts("[ERROR] partial message received\r\n");
            }
        }
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
