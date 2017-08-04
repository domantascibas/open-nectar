#include "mbed.h"
#include "data.h"
#include "comms.h"
#include "power_board.h"

extern RawSerial comms_pc;
extern RawSerial comms_power;
extern RawSerial comms_esp;

extern DigitalOut relay_sun;
extern DigitalOut relay_grid;
extern Data data;

namespace service {
    void init(uint32_t baudrate) {
        comms_pc.baud(baudrate);
    }
    
    void loop(void) {
        uint8_t command, resp;
        while(!comms_pc.readable()) {
        }
        if(comms_pc.readable()) {
            command = comms_pc.getc();
            if(command != NULL) {
                comms_pc.printf("\n\rCMD: 0x%X\r\n", command);
                switch(command) {
                    
                    case KEYBOARD_STOP:
                        comms_pc.printf("POWER BOARD Stopping\r\n");
                        resp = power_board::stop();
                        if(resp == ACK) {
                            comms_pc.printf("Stopped\r\n");
                            relay_sun = false;
                        } else {
                            comms_pc.printf("Not Stopped: 0x%X\r\n", resp);
                        }
                    break;
                    
                    case KEYBOARD_START:
                        comms_pc.printf("POWER BOARD Starting\r\n");
                        resp = power_board::get_calibration_data();
                        if(resp == NS_OK) {
                            comms_pc.printf("[DATA] Calibration V: %.5f I: %.5f\r\n", data.pv_ref_voltage, data.pv_ref_current);
                        }
                    
                        resp = power_board::start();
                        if(resp == ACK) {
                            relay_sun = true;
                            comms_pc.printf("Started\r\n");
                        } else {
                            relay_sun = false;
                            comms_pc.printf("Not Started: 0x%X\r\n", resp);
                        }
                    break;
                        
                    case KEYBOARD_GET_DATA:
                        comms_pc.printf("POWER BOARD Getting data\r\n");
                        resp = power_board::get_data();
                        if(resp == NS_OK) {
                            comms_pc.printf("V:%5.2f I:%5.2f D:%5.2f Tmosfet:%5.2f Overheat:%d Tairgap:%5.2f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp);
                        } else {
                            comms_pc.printf("Error: 0x%X\r\n", resp);
                            comms_pc.printf("V:%5.2f I:%5.2f D:%5.2f Tmosfet:%5.2f Overheat:%d Tairgap:%5.2f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp);
                        }
                    break;
                        
                    case KEYBOARD_GET_CALIB_DATA:
                        comms_pc.printf("POWER BOARD Getting Calibration data\r\n");
                        resp = power_board::get_calibration_data();
                        if(resp == NS_OK) {
                            comms_pc.printf("V_ref:%f I_ref:%f\r\n", data.pv_ref_voltage, data.pv_ref_current);
                        } else {
                            comms_pc.printf("Error: 0x%X\n\r", resp);
                            comms_pc.printf("V_ref:%f I_ref:%f\r\n", data.pv_ref_voltage, data.pv_ref_current);
                        }
                    break;
                        
                    case KEYBOARD_PWM_OFF:
                        comms_pc.printf("POWER BOARD Turning OFF PWM\r\n");
                        resp = power_board::shutdown(DRIVER_OFF);
                        if(resp == ACK) {
                            comms_pc.printf("PWM OFF\r\n");
                            relay_sun = false;
                        } else {
                            comms_pc.printf("Error: 0x%X\r\n", resp);
                        }
                    break;
                        
                    case KEYBOARD_PWM_ON:
                        comms_pc.printf("POWER BOARD Turning ON PWM\r\n");
                        resp = power_board::shutdown(DRIVER_ON);
                        if(resp == ACK) {
                            comms_pc.printf("PWM ON\r\n");
                            relay_sun = true;
                        } else {
                            comms_pc.printf("Error: 0x%X\r\n", resp);
                        }
                    break;
                        
                    case AUTO_MODE:
                        comms_pc.printf("AUTO MODE\r\n");
                        resp = power_board::enter_service_menu(false);
                        if(resp == ACK) {
                            relay_sun = true;
                            comms_pc.printf("Power Board running\r\n");
                        } else {
                            comms_pc.printf("Error: 0x%X\r\n", resp);
                        }
                    break;
                        
                    case MANUAL_MODE:
                        comms_pc.printf("MANUAL MODE\r\n");
                        resp = power_board::enter_service_menu(true);
                        if(resp == NS_OK) {
                            comms_pc.printf("Service Menu\r\nPWM duty: %f\r\n", data.pwm_duty);
                            resp = power_board::get_data();
                            if(resp == NS_OK) {
                                comms_pc.printf("V:%5.2f I:%5.2f D:%5.2f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty);
                            } else {
                                comms_pc.printf("Error: 0x%X\r\n", resp);
                            }
                        } else {
                            comms_pc.printf("Error: 0x%X\r\n", resp);
                        }
                    break;
                        
                    case INCREASE_PWM_DUTY:
                        comms_pc.printf("INCREASE DUTY\r\n");
                        resp = power_board::increase_pwm(true);
                        if(resp == NS_OK) {
                            resp = power_board::get_data();
                            if(resp == NS_OK) {
                                comms_pc.printf("V:%5.2f I:%5.2f D:%5.2f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty);
                            } else {
                                comms_pc.printf("Error: 0x%X\r\n", resp);
                            }
                        } else {
                            comms_pc.printf("Error: 0x%X\r\n", resp);
                        }
                    break;
                        
                    case DECREASE_PWM_DUTY:
                        comms_pc.printf("DECREASE DUTY\r\n");
                        resp = power_board::increase_pwm(false);
                        if(resp == NS_OK) {
                            resp = power_board::get_data();
                            if(resp == NS_OK) {
                                comms_pc.printf("V:%5.2f I:%5.2f D:%5.2f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty);
                            } else {
                                comms_pc.printf("Error: 0x%X\r\n", resp);
                            }
                        } else {
                            comms_pc.printf("Error: 0x%X\r\n", resp);
                        }
                    break;
                        
                    case GET_STATS:
                        //send stats to ESP
                        comms_pc.printf("ESP GET STATS\r\n");
                        //esp::get_stats();
                    break;

                    default:
                        comms_pc.printf("Unrecognized CMD 0x%X\r\n", command);
                    break;
                }
            } else {
                comms_pc.printf("CMD: NULL\r\n");
            }
        }
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
