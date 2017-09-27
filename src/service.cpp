#include "mbed.h"
#include "data.h"
#include "comms.h"
#include "service.h"
#include "power_board.h"

static const PinName TX = PA_2;
static const PinName RX = PA_3;

/*
static const uint8_t POWER_BOARD_STOP = 0x30;   //'0'
static const uint8_t POWER_BOARD_START = 0x31;  //'1'
static const uint8_t GET_DATA = 0x32;           //'2'
static const uint8_t GET_REF_DATA = 0x33;       //'3'
static const uint8_t GET_PWM = 0x37;            //'7'
static const uint8_t PWM_OFF = 0x38;            //'8'
static const uint8_t PWM_ON = 0x39;             //'9'
static const uint8_t INCREASE_PWM = 0x2B;       //'+'
static const uint8_t DECREASE_PWM = 0x2D;       //'-'
static const uint8_t AUTO_MODE = 0x61;          //'a'
static const uint8_t MANUAL_MODE = 0x6D;        //'m'
*/

static const uint8_t POWER_BOARD_STOP = 0x30;       //'0'
static const uint8_t POWER_BOARD_START = 0x31;      //'1'
static const uint8_t GET_DATA = 0x32;               //'2'
static const uint8_t GET_REF_DATA = 0x33;           //'3'
static const uint8_t ENTER_SERVICE_MODE = 0x6D;     //'m'

static const uint8_t INCREASE_PWM = 0x2B;           //'+'
static const uint8_t DECREASE_PWM = 0x2D;           //'-'
static const uint8_t CLEAR_ERROR = 0x20;            //'SPACE'

RawSerial pc(TX, RX);
extern Ticker get_data_tick;
static const float interval = 1.0;

namespace service {
  volatile char command = NULL;
  
  void Rx_interrupt() {
    //comms::send_command(ENTER_SERVICE_MODE);
    command = pc.getc();
    pc.putc(command);
  }
  
  void setup() {
    static const uint32_t baudrate = 115200;
    
    pc.baud(baudrate);
    pc.printf("[COMMS] Started\r\n");
    //pc.attach(&Rx_interrupt);
  }
  
  void loop() {
//    if(command != NULL) {
//      switch(command) {
//        case POWER_BOARD_STOP:
//          //printf("stopping\r\n");
//          comms::send_command(POWER_BOARD_STOP);
//        break;
//        
//        case POWER_BOARD_START:
//          //printf("starting\r\n");
//          comms::send_command(POWER_BOARD_START);
//          //get_data_tick.attach(&power_board::get_data_ISR, interval);
//        break;
//        
//        case CLEAR_ERROR:
//          //printf("clearing error\r\n");
//          comms::send_command(CLEAR_ERROR);
//        break;
//        
//        case ENTER_SERVICE_MODE:
//          //printf("entering service mode\r\n");
//          comms::send_command(ENTER_SERVICE_MODE);
//          get_data_tick.detach();
//        break;
//        
//        case INCREASE_PWM:
//          //printf("increase pwm\r\n");
//          comms::send_command(INCREASE_PWM);
//        break;
//        
//        case DECREASE_PWM:
//          //printf("decrease pwm\r\n");
//          comms::send_command(DECREASE_PWM);
//        break;
//        
//        case GET_DATA:
//          //printf("get data\r\n");
//          comms::send_command(GET_DATA);
//        break;
//        
//        case GET_REF_DATA:
//          //printf("get ref data\r\n");
//          comms::send_command(GET_REF_DATA);
//        break;
//        
//        default:
//          printf("Unrecognized command 0x%X\r\n", command);
//        break;
//      }
//      command = NULL;
//    }
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
  
//    void init(uint32_t baudrate) {
//        comms_pc.baud(baudrate);
//    }
//    
//    void loop(void) {
//        uint8_t command, resp;
//        while(!comms_pc.readable()) {
//        }
//        if(comms_pc.readable()) {
//            command = comms_pc.getc();
//            if(command != NULL) {
//                comms_pc.printf("\n\rCMD: 0x%X\r\n", command);
//                switch(command) {
//                    
//                    case KEYBOARD_STOP:
//                        comms_pc.printf("POWER BOARD Stopping\r\n");
//                        resp = power_board::stop();
//                        if(resp == ACK) {
//                            comms_pc.printf("Stopped\r\n");
//                            relay_sun = false;
//                        } else {
//                            comms_pc.printf("Not Stopped: 0x%X\r\n", resp);
//                        }
//                    break;
//                    
//                    case KEYBOARD_START:
//                        comms_pc.printf("POWER BOARD Starting\r\n");
//                        resp = power_board::get_calibration_data();
//                        if(resp == NS_OK) {
//                            comms_pc.printf("[DATA] Calibration V: %.5f I: %.5f\r\n", data.pv_ref_voltage, data.pv_ref_current);
//                        }
//                    
//                        resp = power_board::start();
//                        if(resp == ACK) {
//                            relay_sun = true;
//                            comms_pc.printf("Started\r\n");
//                        } else {
//                            relay_sun = false;
//                            comms_pc.printf("Not Started: 0x%X\r\n", resp);
//                        }
//                    break;
//                        
//                    case KEYBOARD_GET_DATA:
//                        comms_pc.printf("POWER BOARD Getting data\r\n");
//                        resp = power_board::get_data();
//                        if(resp == NS_OK) {
//                            comms_pc.printf("V:%5.2f I:%5.2f D:%5.2f Tmosfet:%5.2f Overheat:%d Tairgap:%5.2f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp);
//                        } else {
//                            comms_pc.printf("Error: 0x%X\r\n", resp);
//                            comms_pc.printf("V:%5.2f I:%5.2f D:%5.2f Tmosfet:%5.2f Overheat:%d Tairgap:%5.2f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp);
//                        }
//                    break;
//                        
//                    case KEYBOARD_GET_CALIB_DATA:
//                        comms_pc.printf("POWER BOARD Getting Calibration data\r\n");
//                        resp = power_board::get_calibration_data();
//                        if(resp == NS_OK) {
//                            comms_pc.printf("V_ref:%f I_ref:%f\r\n", data.pv_ref_voltage, data.pv_ref_current);
//                        } else {
//                            comms_pc.printf("Error: 0x%X\n\r", resp);
//                            comms_pc.printf("V_ref:%f I_ref:%f\r\n", data.pv_ref_voltage, data.pv_ref_current);
//                        }
//                    break;
//                        
//                    case KEYBOARD_PWM_OFF:
//                        comms_pc.printf("POWER BOARD Turning OFF PWM\r\n");
//                        resp = power_board::shutdown(DRIVER_OFF);
//                        if(resp == ACK) {
//                            comms_pc.printf("PWM OFF\r\n");
//                            relay_sun = false;
//                        } else {
//                            comms_pc.printf("Error: 0x%X\r\n", resp);
//                        }
//                    break;
//                        
//                    case KEYBOARD_PWM_ON:
//                        comms_pc.printf("POWER BOARD Turning ON PWM\r\n");
//                        resp = power_board::shutdown(DRIVER_ON);
//                        if(resp == ACK) {
//                            comms_pc.printf("PWM ON\r\n");
//                            relay_sun = true;
//                        } else {
//                            comms_pc.printf("Error: 0x%X\r\n", resp);
//                        }
//                    break;
//                        
//                    case AUTO_MODE:
//                        comms_pc.printf("AUTO MODE\r\n");
//                        resp = power_board::enter_service_menu(false);
//                        if(resp == ACK) {
//                            relay_sun = true;
//                            comms_pc.printf("Power Board running\r\n");
//                        } else {
//                            comms_pc.printf("Error: 0x%X\r\n", resp);
//                        }
//                    break;
//                        
//                    case MANUAL_MODE:
//                        comms_pc.printf("MANUAL MODE\r\n");
//                        resp = power_board::enter_service_menu(true);
//                        if(resp == NS_OK) {
//                            comms_pc.printf("Service Menu\r\nPWM duty: %f\r\n", data.pwm_duty);
//                            resp = power_board::get_data();
//                            if(resp == NS_OK) {
//                                comms_pc.printf("V:%5.2f I:%5.2f D:%5.2f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty);
//                            } else {
//                                comms_pc.printf("Error: 0x%X\r\n", resp);
//                            }
//                        } else {
//                            comms_pc.printf("Error: 0x%X\r\n", resp);
//                        }
//                    break;
//                        
//                    case INCREASE_PWM_DUTY:
//                        comms_pc.printf("INCREASE DUTY\r\n");
//                        resp = power_board::increase_pwm(true);
//                        if(resp == NS_OK) {
//                            resp = power_board::get_data();
//                            if(resp == NS_OK) {
//                                comms_pc.printf("V:%5.2f I:%5.2f D:%5.2f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty);
//                            } else {
//                                comms_pc.printf("Error: 0x%X\r\n", resp);
//                            }
//                        } else {
//                            comms_pc.printf("Error: 0x%X\r\n", resp);
//                        }
//                    break;
//                        
//                    case DECREASE_PWM_DUTY:
//                        comms_pc.printf("DECREASE DUTY\r\n");
//                        resp = power_board::increase_pwm(false);
//                        if(resp == NS_OK) {
//                            resp = power_board::get_data();
//                            if(resp == NS_OK) {
//                                comms_pc.printf("V:%5.2f I:%5.2f D:%5.2f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty);
//                            } else {
//                                comms_pc.printf("Error: 0x%X\r\n", resp);
//                            }
//                        } else {
//                            comms_pc.printf("Error: 0x%X\r\n", resp);
//                        }
//                    break;
//                        
//                    case GET_STATS:
//                        //send stats to ESP
//                        comms_pc.printf("ESP GET STATS\r\n");
//                        //esp::get_stats();
//                    break;

//                    default:
//                        comms_pc.printf("Unrecognized CMD 0x%X\r\n", command);
//                    break;
//                }
//            } else {
//                comms_pc.printf("CMD: NULL\r\n");
//            }
//        }
//    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
