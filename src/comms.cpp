#include "mbed.h"
#include "rtos.h"
#include "data.h"
#include "comms.h"
#include <string>
#include <sstream>
#include <iostream>

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
#define INCOMING_DATA               0x25    //'%'
#define WAITING_FOR_DATA            0x26    //'&'
#define BAD_CMD                     0x21    //'!'
#define NUM_FIELDS                  12 //number of comma seperated values in the data...TODO does this remain constant?
#define HANDSHAKE                   0x5E

extern Serial comms_pc;
extern Serial comms_power;
extern Serial comms_esp;
extern DigitalOut relay_sun;
extern DigitalOut relay_grid;

Mutex serial;

extern Data data;
char* pFields[NUM_FIELDS];

struct Message_Result {
    float float_result;
    uint8_t int_result;
};

Message_Result result = {0.0, 0};

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
        }
        if(comms_pc.readable()) {
            serial.lock();
            
            command = comms_pc.getc();
            if(command != NULL) {
                comms_pc.printf("\n\rCMD: 0x%X\n\r", command);
                switch(command) {
                    
                    case KEYBOARD_STOP:
                        comms_pc.printf("POWER BOARD Stopping\n\r");
                        resp = power_board::stop();
                        if(resp == ACK) {
                            comms_pc.printf("Stopped\n\r");
                            relay_sun = false;
                        } else {
                            comms_pc.printf("Not Stopped: 0x%X\n\r", resp);
                        }
                    break;
                    
                    case KEYBOARD_START:
                        comms_pc.printf("POWER BOARD Starting\n\r");
                        relay_sun = true;
                        resp = power_board::start();
                        if(resp == ACK) {
                            comms_pc.printf("Started\n\r");
                        } else {
                            relay_sun = false;
                            comms_pc.printf("Not Started: 0x%X\n\r", resp);
                        }
                    break;
                        
                    case KEYBOARD_GET_DATA:
                        comms_pc.printf("POWER BOARD Getting data\n\r");
                        resp = power_board::get_data();
                        if(resp == NS_OK) {
                            comms_pc.printf("V:%7.3f I:%7.3f D:%5.2f Tmosfet:%7.3f Overheat:%d Tairgap:%7.3f\n\r", data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp);
                        } else {
                            comms_pc.printf("Error: 0x%X\n\r", resp);
                            comms_pc.printf("V:%7.3f I:%7.3f D:%5.2f Tmosfet:%7.3f Overheat:%d Tairgap:%7.3f\n\r", data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp);
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
                            relay_sun = false;
                        } else {
                            comms_pc.printf("Error: 0x%X\n\r", resp);
                        }
                    break;
                        
                    case KEYBOARD_PWM_ON:
                        comms_pc.printf("POWER BOARD Turning ON PWM\n\r");
                        resp = power_board::shutdown(DRIVER_ON);
                        if(resp == ACK) {
                            comms_pc.printf("PWM ON\n\r");
                            relay_sun = true;
                        } else {
                            comms_pc.printf("Error: 0x%X\n\r", resp);
                        }
                    break;
                        
                    case AUTO_MODE:
                        comms_pc.printf("AUTO MODE\n\r");
                        resp = power_board::enter_service_menu(false);
                        if(resp == ACK) {
                            relay_sun = true;
                            comms_pc.printf("Power Board running\n\r");
                        } else {
                            comms_pc.printf("Error: 0x%X\n\r", resp);
                        }
                    break;
                        
                    case MANUAL_MODE:
                        comms_pc.printf("MANUAL MODE\n\r");
                        resp = power_board::enter_service_menu(true);
                        if(resp == NS_OK) {
                            comms_pc.printf("Service Menu\n\rPWM duty: %f\r\n", data.pwm_duty);
                            resp = power_board::get_data();
                            if(resp == NS_OK) {
                                comms_pc.printf("V:%7.3f I:%7.3f D:%5.2f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty);
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
                                comms_pc.printf("V:%7.3f I:%7.3f D:%5.2f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty);
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
                                comms_pc.printf("V:%7.3f I:%7.3f D:%5.2f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty);
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
        
        serial.unlock();
    }
}

namespace esp {
    void init(uint32_t baudrate) {
        uint8_t resp;
        comms_esp.baud(baudrate);
    }
    
    //sends data back to ESP as a string
    //  temperature_moment,
    //  sun_power_moment,
    //  sun_voltage,
    //  sun_current,
    //  pwm_duty,
    //  device_temperature,
    //   erature,
    //  sun_relay_on,
    //  grid_relay_on,
    //  transistor_overheat_on
    uint8_t get_stats(void) {
        comms_pc.printf("sent 0x%X\r\n", INCOMING_DATA);
        //comms_esp.putc(INCOMING_DATA);
        //can send 64 chars max
        comms_esp.printf("#%.2f,%d,%.2f,%d,%.2f,%.2f,%.2f,%d,%.2f,%.2f,%.2f$\n", data.pv_power, data.grid_relay_on, data.temp_boiler, data.sun_relay_on, data.pv_voltage, data.pv_current, data.device_temperature, data.mosfet_overheat_on, data.radiator_temp, data.pwm_duty, data.airgap_temp);
        //comms_pc.printf("<%.2f,%d,%.2f,%d,%.2f,%.2f,%.2f,%d,%.2f,%.2f>\n\r", data.pv_power, data.grid_relay_on, data.temp_boiler, data.sun_relay_on, data.pv_voltage, data.pv_current, data.device_temperature, data.mosfet_overheat_on, data.radiator_temp, data.pwm_duty);
        comms_pc.printf("V:%7.3f I:%7.3f D:%5.2f Tmosfet:%7.3f Overheat:%d Tairgap:%7.3f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp);
    }
    
    void loop(void) {
        uint8_t command, resp;
        
        while(!comms_esp.readable()) {
        }
        if(comms_esp.readable()) {
            
            serial.lock();
            command = comms_esp.getc();
            if(command != NULL) {
                comms_pc.printf("\r\nCMD: 0x%X\r\n", command);
                switch(command) {
                    case GET_STATS:
                        comms_pc.printf("POWER BOARD Getting data\n\r");
                        resp = power_board::get_data();
                        if(resp == NS_OK) {
                            //comms_pc.printf("V:%7.3f I:%7.3f D:%5.2f Tmosfet:%7.3f Overheat:%d Tairgap:%7.3f\n\r", data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp);
                            //send stats to ESP
                            comms_pc.printf("ESP GET STATS\n\r");
                            get_stats();
                        } else {
                            comms_pc.printf("Error: 0x%X\n\r", resp);
                            comms_pc.printf("V:%7.3f I:%7.3f D:%5.2f Tmosfet:%7.3f Overheat:%d Tairgap:%7.3f\n\r", data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp);
                            comms_esp.putc(NACK);
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
        
        serial.unlock();
    }
}

namespace power_board {
    //char buffer[128];

    void init(uint32_t baudrate) {
        comms_power.baud(baudrate);
    }
    
    uint8_t start(void) {
        return send_cmd(CMD_POWER_BOARD_START);
    }
    
    uint8_t stop(void) {
        return send_cmd(CMD_POWER_BOARD_STOP);
    }
    
    uint8_t get_data(void) {
        uint8_t response = send_cmd(KEYBOARD_GET_DATA);
        if(response == INCOMING_DATA) {
            comms_power.scanf("#%f,%f,%f,%f,%d,%f$", &data.pv_voltage, &data.pv_current, &data.pwm_duty, &data.radiator_temp, &data.mosfet_overheat_on, &data.airgap_temp);
            comms_power.getc();
            data.pv_power = data.pv_voltage * data.pv_current;
            if(data.pv_power > 5.00) {
                data.pv_available = true;
            } else {
                data.pv_available = false;
            }
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
            comms_power.scanf("#%f,%f$", &data.pv_ref_voltage, &data.pv_ref_current);
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
                    comms_power.scanf("#%f$", &data.pwm_duty);
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
            comms_power.printf("#%f$\n", data.pwm_duty);
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
