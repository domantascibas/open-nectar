#include "mbed.h"
#include "rtos.h"
#include "data.h"
#include "comms.h"
#include <string>
#include <sstream>
#include <iostream>

//Serial com pins
#define PC_TX                       PA_2
#define PC_RX                       PA_3
#define POWER_TX                    PB_10
#define POWER_RX                    PB_11
#define ESP_TX                      PA_9
#define ESP_RX                      PA_10

//Commands
#define KEYBOARD_STOP               0x30    //'0'
#define KEYBOARD_START              0x31    //'1'
#define KEYBOARD_GET_DATA           0x32    //'2'
#define KEYBOARD_GET_CALIB_DATA     0x33    //'3'
#define KEYBOARD_PWM_OFF            0x38    //'8'
#define KEYBOARD_PWM_ON             0x39    //'9'
#define INCREASE_PWM_DUTY           0x2B    //'+'
#define DECREASE_PWM_DUTY           0x2D    //'-'
#define AUTO_MODE                   0x61    //'a'
#define MANUAL_MODE                 0x6D    //'m'

#define CMD_POWER_BOARD_START       0x0F
#define CMD_POWER_BOARD_STOP        0xF0

#define CMD_GET_VOLTAGE             0x56    //'V'
#define CMD_GET_CURRENT             0x49    //'I'
#define CMD_GET_REF_VOLTAGE         0x4B    //'K'
#define CMD_GET_REF_CURRENT         0x4C    //'L'

#define CMD_SET_PWM_DUTY            0x44    //'D'
#define CMD_GET_PWM_DUTY            0x45    //'E'
#define CMD_GET_CAP_TEMP            0x4E    //'P'
#define CMD_GET_MOSFET_OHEAT        0x4F    //'O'

#define CMD_PWM_ON                  0x41    //'A'
#define CMD_PWM_OFF                 0x42    //'B'

#define SET_MODE                    0x4D    //'M'
#define GET_STATS                   0x54    //'T'

//Constants
#define DRIVER_ON                   0
#define DRIVER_OFF                  1
#define ACK                         0x06
#define NACK                        0x15
#define INCOMING_DATA               0x24    //'$'
#define WAITING_FOR_DATA            0x23    //'#'
#define NUM_FIELDS                  12 //number of comma seperated values in the data...TODO does this remain constant?

Serial comms_pc(PC_TX, PC_RX);
Serial comms_power(POWER_TX, POWER_RX);
Serial comms_esp(ESP_TX, ESP_RX);
extern DigitalOut relay_sun_on;
extern DigitalOut relay_grid_on;
extern Mutex serial;

extern Data data;
char* pFields[NUM_FIELDS];

struct Message_Result {
    float float_result;
    uint8_t int_result;
};

Message_Result result = {0.0, 0};

enum data_types {
    INTEGER,
    FLOAT
};

char create_string(ostringstream message) {
    char new_string[128];
    string s = message.str();
    const char* p = s.c_str();
    strlcpy(new_string, p, sizeof(new_string) - 1);
    return *new_string;
}

//parses the received message and saves data into the pFields array
uint8_t parse_fields(char* inputBuffer, char** pFields, uint32_t numFields, char delimiterChars) {
    char* pString = inputBuffer;
    char* pField;
    uint8_t length = 0;
    comms_pc.printf("[ PARSING ] %s\n\r", inputBuffer);
    for(uint8_t i=0; i<numFields; i++) {
        pField = strtok(pString, &delimiterChars);
        if(pField != NULL) {
            pFields[i] = pField;
            length++;
        } else {
            pFields[i] = NULL;
        }
        pString = NULL; //to make strtok continue parsing the next field rather than start again on the original string (see strtok documentation for more details)
    }
    comms_pc.printf("parsed %d values\n\r", length);
    return length;
}

uint8_t send_cmd(uint8_t command) {
    //TODO add timeout after sending command
    uint8_t response;
    comms_power.putc(command);
    response = comms_power.getc();
    if(response == NACK) {
        response = comms_power.getc();
        comms_pc.printf("error 0x%X\t", response);
    }
    return response;
}

//if the received command is in the list of commands, respond with ACK
//send data back
//if the command is unknown respond with NACK
//send the error code back
uint8_t receive_cmd(uint8_t command) {
}

namespace pc_monitor {
    void init(uint32_t baudrate) {
        comms_pc.baud(baudrate);
    }
    
    void loop(void) {
        uint8_t command, resp;
        while(!comms_pc.readable()) {
            __WFI();
        }
        if(comms_pc.readable()) {
            command = comms_pc.getc();
            if(command != NULL) {
                comms_pc.printf("\n\rCMD: 0x%X\n\r", command);
                switch(command) {
                    case KEYBOARD_STOP:
                        comms_pc.printf("POWER BOARD Stopping\n\r");
                        resp = power_board::stop();
                        if(resp == ACK) {
                            comms_pc.printf("Stopped\n\r");
                            relay_sun_on = false;
                        } else {
                            comms_pc.printf("Not Stopped: 0x%X\n\r", resp);
                        }
                    break;
                    
                    case KEYBOARD_START:
                        comms_pc.printf("POWER BOARD Starting\n\r");
                        relay_sun_on = true;
                        resp = power_board::start();
                        if(resp == ACK) {
                            comms_pc.printf("Started\n\r");
                        } else {
                            relay_sun_on = false;
                            comms_pc.printf("Not Started: 0x%X\n\r", resp);
                        }
                    break;
                        
                    case KEYBOARD_GET_DATA:
                        comms_pc.printf("POWER BOARD Getting data\n\r");
                        resp = power_board::get_data();
                        if(resp == NS_OK) {
                            comms_pc.printf("V:%7.3f I:%7.3f D:%5.2f Tcap:%7.3f Overheat:%d\n\r", data.pv_voltage, data.pv_current, data.pwm_duty, data.capacitor_temp, data.mosfet_overheat_on);
                        } else {
                            comms_pc.printf("Error: 0x%X\n\r", resp);
                            comms_pc.printf("V:%7.3f I:%7.3f D:%5.2f Tcap:%7.3f Overheat:%d\n\r", data.pv_voltage, data.pv_current, data.pwm_duty, data.capacitor_temp, data.mosfet_overheat_on);
                        }
                    break;
                        
                    case KEYBOARD_GET_CALIB_DATA:
                        comms_pc.printf("POWER BOARD Getting Calibration data\n\r");
                        resp = power_board::get_calibration_data();
                        if(resp == NS_OK) {
                            comms_pc.printf("V_ref:%f I_ref:%f\n\r", data.pv_ref_voltage, data.pv_ref_current);
                        } else {
                            comms_pc.printf("Error: 0x%X\n\r", resp);
                            comms_pc.printf("V_ref:%f I_ref:%f\n\r", data.pv_ref_voltage, data.pv_ref_current);
                        }
                    break;
                        
                    case KEYBOARD_PWM_OFF:
                        comms_pc.printf("POWER BOARD Turning OFF PWM\n\r");
                        resp = power_board::shutdown(DRIVER_OFF);
                        if(resp == ACK) {
                            comms_pc.printf("PWM OFF\n\r");
                            relay_sun_on = false;
                        } else {
                            comms_pc.printf("Error: 0x%X\n\r", resp);
                        }
                    break;
                        
                    case KEYBOARD_PWM_ON:
                        comms_pc.printf("POWER BOARD Turning ON PWM\n\r");
                        resp = power_board::shutdown(DRIVER_ON);
                        if(resp == ACK) {
                            comms_pc.printf("PWM ON\n\r");
                            relay_sun_on = true;
                        } else {
                            comms_pc.printf("Error: 0x%X\n\r", resp);
                        }
                    break;
                        
                    case AUTO_MODE:
                        comms_pc.printf("AUTO MODE\n\r");
                        resp = power_board::enter_service_menu(false);
                        if(resp == ACK) {
                            relay_sun_on = true;
                            comms_pc.printf("Power Board running\n\r");
                        } else {
                            comms_pc.printf("Error: 0x%X\n\r", resp);
                        }
                    break;
                        
                    case MANUAL_MODE:
                        comms_pc.printf("MANUAL MODE\n\r");
                        resp = power_board::enter_service_menu(true);
                        if(resp == NS_OK) {
                            comms_pc.printf("Service Menu\n\rPWM duty: %f\n\r", data.pwm_duty);
                            resp = power_board::get_data();
                            if(resp == NS_OK) {
                                comms_pc.printf("V:%7.3f I:%7.3f D:%5.2f\n\r", data.pv_voltage, data.pv_current, data.pwm_duty);
                            } else {
                                comms_pc.printf("Error: 0x%X\n\r", resp);
                            }
                        } else {
                            comms_pc.printf("Error: 0x%X\n\r", resp);
                        }
                    break;
                        
                    case INCREASE_PWM_DUTY:
                        comms_pc.printf("INCREASE DUTY\n\r");
                        resp = power_board::increase_pwm(true);
                        if(resp == NS_OK) {
                            resp = power_board::get_data();
                            if(resp == NS_OK) {
                                comms_pc.printf("V:%7.3f I:%7.3f D:%5.2f\n\r", data.pv_voltage, data.pv_current, data.pwm_duty);
                            } else {
                                comms_pc.printf("Error: 0x%X\n\r", resp);
                            }
                        } else {
                            comms_pc.printf("Error: 0x%X\n\r", resp);
                        }
                    break;
                        
                    case DECREASE_PWM_DUTY:
                        comms_pc.printf("DECREASE DUTY\n\r");
                        resp = power_board::increase_pwm(false);
                        if(resp == NS_OK) {
                            resp = power_board::get_data();
                            if(resp == NS_OK) {
                                comms_pc.printf("V:%7.3f I:%7.3f D:%5.2f\n\r", data.pv_voltage, data.pv_current, data.pwm_duty);
                            } else {
                                comms_pc.printf("Error: 0x%X\n\r", resp);
                            }
                        } else {
                            comms_pc.printf("Error: 0x%X\n\r", resp);
                        }
                    break;

                    default:
                        comms_pc.printf("Unrecognized CMD 0x%X\n\r", command);
                    break;
                }
            } else {
                comms_pc.printf("CMD: NULL\n\r");
            }
        }
    }
}

namespace esp {
    void init(uint32_t baudrate) {
        comms_esp.baud(baudrate);
    }
}

namespace power_board {
    char buffer[128];
    
    void init(uint32_t baudrate) {
        comms_power.baud(baudrate);
    }
    
    uint8_t start(void) {
        return send_cmd(CMD_POWER_BOARD_START);
    }
    
    uint8_t stop(void) {
        return send_cmd(CMD_POWER_BOARD_STOP);
    }
    
    uint8_t get_float_data(uint8_t command, float *result) {
        uint8_t response = send_cmd(command);
        if(response == INCOMING_DATA) {
            while(!comms_power.readable()) {
            }
            comms_power.scanf("%f", result);
            comms_power.getc();
            return NS_OK;
        } else {
            return response;
        }
    }
    
    uint8_t get_int_data(uint8_t command, uint8_t *result) {
        uint8_t response = send_cmd(command);
        if(response == INCOMING_DATA) {
            while(!comms_power.readable()) {
            }
            comms_power.scanf("%d", result);
            comms_power.getc();
            return NS_OK;
        } else {
            return response;
        }
    }
    
    uint8_t send_float_data(uint8_t command, float data) {
        uint8_t response = send_cmd(command);
        if(response == WAITING_FOR_DATA) {
            comms_power.printf("%f\n", data);
            return NS_OK;
        } else {
            return response;
        }
    }
    
    uint8_t get_data(void) {
        uint8_t response = send_cmd(KEYBOARD_GET_DATA);
        if(response == INCOMING_DATA) {
            while(!comms_power.readable()) {
            }
            comms_power.scanf("%f,%f,%f,%f,%d", &data.pv_voltage, &data.pv_current, &data.pwm_duty, &data.capacitor_temp, &data.mosfet_overheat_on);
            comms_power.getc();
            return NS_OK;
        } else {
            return response;
        }
    }
    
    uint8_t get_calibration_data(void) {
        uint8_t response = send_cmd(KEYBOARD_GET_CALIB_DATA);
        if(response == INCOMING_DATA) {
            while(!comms_power.readable()) {
            }
            comms_power.scanf("%f,%f", &data.pv_ref_voltage, &data.pv_ref_current);
            comms_power.getc();
            return NS_OK;
        } else {
            return response;
        }
    }

    uint8_t shutdown(bool status_off) {
        if(status_off) {
            //DRIVER_OFF
            return send_cmd(CMD_PWM_OFF);
        } else {
            //DRIVER_ON
            return send_cmd(CMD_PWM_ON);
        }
    }
    
    uint8_t enter_service_menu(bool service_menu) {
        uint8_t response;
        if(service_menu) {
            response = send_cmd(MANUAL_MODE);
            if(response == ACK) {
                response = send_cmd(CMD_GET_PWM_DUTY);
                if(response == INCOMING_DATA) {
                    while(!comms_power.readable()) {}
                    comms_power.scanf("%f", &data.pwm_duty);
                    comms_power.getc();
                    return NS_OK;
                }else {
                    return response;
                }
            } else {
                return response;
            }
        } else {
            return send_cmd(AUTO_MODE);
        }
    }
    
    uint8_t increase_pwm(bool increase) {
        if(data.pwm_duty < 0.1) {
            data.pwm_duty = 0.1;
        }
        uint8_t response = send_cmd(CMD_SET_PWM_DUTY);
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
            comms_power.printf("%f\n", data.pwm_duty);
            comms_pc.printf("set PWM duty %f\n\r", data.pwm_duty);
            return NS_OK;
        } else {
            comms_pc.printf("Error: 0x%X\n\r", response);
        }
    }
}

namespace power_board_tests {
/*
    uint8_t set_pwm(float duty) {
        comms_power.putc(SET_PWM_DUTY);
        comms_power.printf("%f\n", duty);
        
        return 0;
    }

    uint8_t shutdown(bool status_off) {
        if(status_off) {
            comms_power.putc(PWM_DRIVER_OFF);
        } else {
            comms_power.putc(PWM_DRIVER_ON);
        }
        return 0;
    }
*/
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
