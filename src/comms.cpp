#include "mbed.h"
#include "rtos.h"
#include "data.h"
#include "comms.h"
#include <string>
#include <sstream>
#include <iostream>

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

extern RawSerial comms_pc;
extern RawSerial comms_power;
extern RawSerial comms_esp;
extern DigitalOut relay_sun;
extern DigitalOut relay_grid;
Mutex serial;

extern Data data;
void error_message(uint8_t);

char* pFields[NUM_FIELDS];
uint8_t numChars = 128;
char receivedChars[128];
char tempChars[128];        // temporary array for use when parsing
bool newData = false;

uint8_t send_cmd(uint8_t command) {
    //TODO add timeout after sending command
    uint8_t response;
    comms_power.putc(command);
    response = comms_power.getc();
    if(response == NACK) {
        response = comms_power.getc();
        error_message(response);
        //if(comms_power.getc() == INCOMING_DATA) {
        //    comms_power.scanf("#%f,%f,%f,%f,%d,%f$", &data.pv_voltage, &data.pv_current, &data.pwm_duty, &data.radiator_temp, &data.mosfet_overheat_on, &data.airgap_temp);
        //    comms_power.getc();
        //    comms_pc.printf("V:%7.3f I:%7.3f D:%5.2f Tmosfet:%7.3f Overheat:%d Tairgap:%7.3f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp);
        //}
    }
    return response;
}

//parses the received message and saves data into the pFields array
uint8_t parse_fields(char* inputBuffer, char** pFields, uint32_t numFields, char delimiterChars) {
    char* pString = inputBuffer;
    char* pField;
    uint8_t length = 0;
    comms_pc.printf("[PARSING] ");
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
    comms_pc.printf("parsed %d values\r\n", length);
    return length;
}

void receive_message() {
    bool recvInProgress = false;
    uint8_t ndx = 0;
    char startMarker = '#';
    char endMarker = '$';
    char rc;
    
    while(!comms_power.readable()) {}
    while(!newData) {
        rc = comms_power.getc();
        //comms_pc.putc(rc);

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            } else {
                receivedChars[ndx] = '\0';  // terminate the string
                comms_power.getc();         //take the \n char from the serial bus
                recvInProgress = false;
                ndx = 0;
                newData = true;
                //comms_pc.printf("\r\n");
            }
        } else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
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
            serial.unlock();
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
                        serial.lock();
                        comms_pc.printf("POWER BOARD Getting data\r\n");
                        resp = power_board::get_data();
                        if(resp == NS_OK) {
                            comms_pc.printf("V:%5.2f I:%5.2f D:%5.2f Tmosfet:%5.2f Overheat:%d Tairgap:%5.2f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp);
                        } else {
                            comms_pc.printf("Error: 0x%X\r\n", resp);
                            comms_pc.printf("V:%5.2f I:%5.2f D:%5.2f Tmosfet:%5.2f Overheat:%d Tairgap:%5.2f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp);
                        }
                        serial.unlock();
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
        serial.lock();
        comms_esp.printf("#%.2f,%d,%.2f,%d,%.2f,%.2f,%.2f,%d,%.2f,%.2f,%.2f$\n", data.pv_power, data.grid_relay_on, data.temp_boiler, data.sun_relay_on, data.pv_voltage, data.pv_current, data.device_temperature, data.mosfet_overheat_on, data.radiator_temp, data.pwm_duty, data.airgap_temp);
        //comms_pc.printf("<%.2f,%d,%.2f,%d,%.2f,%.2f,%.2f,%d,%.2f,%.2f>\n\r", data.pv_power, data.grid_relay_on, data.temp_boiler, data.sun_relay_on, data.pv_voltage, data.pv_current, data.device_temperature, data.mosfet_overheat_on, data.radiator_temp, data.pwm_duty);
        //comms_pc.printf("P:%.2f GRID:%d Twater:%.2f SUN:%d V:%.2f I:%.2f Tdevice:%.2f MOSFET:%d Trad:%.2f Duty:%.2f Tairgap:%.2f\r\n", data.pv_power, data.grid_relay_on, data.temp_boiler, data.sun_relay_on, data.pv_voltage, data.pv_current, data.device_temperature, data.mosfet_overheat_on, data.radiator_temp, data.pwm_duty, data.airgap_temp);
        comms_pc.printf("V:%5.2f I:%5.2f D:%5.2f Tmosfet:%5.2f Overheat:%d Tairgap:%5.2f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp);
        serial.unlock();
    }
    
    void loop(void) {
        uint8_t command, resp;
        if(comms_esp.readable()) {
            command = comms_esp.getc();
            if(command != NULL) {
                comms_pc.printf("\r\nCMD: 0x%X\r\n", command);
                switch(command) {
                    case GET_STATS:
                        comms_pc.printf("POWER BOARD Getting data\r\n");
                        resp = power_board::get_data();
                        if(resp == NS_OK) {
                            //comms_pc.printf("V:%7.3f I:%7.3f D:%5.2f Tmosfet:%7.3f Overheat:%d Tairgap:%7.3f\n\r", data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp);
                            //send stats to ESP
                            comms_pc.printf("ESP GET STATS\r\n");
                            get_stats();
                        } else {
                            comms_pc.printf("Error: 0x%X\r\n", resp);
                            comms_pc.printf("V:%5.2f I:%5.2f D:%5.2f Tmosfet:%5.2f Overheat:%d Tairgap:%5.2f\n\r", data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp);
                            comms_esp.putc(NACK);
                        }
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
        uint8_t value_count;
        uint8_t response = send_cmd(KEYBOARD_GET_DATA);
        if(response == INCOMING_DATA) {
            receive_message();
            if (newData == true) {
                newData = false;
                strcpy(tempChars, receivedChars);
                value_count = parse_fields(receivedChars, pFields, NUM_FIELDS, ',');
                //comms_power.scanf("#%f,%f,%f,%f,%d,%f$", &data.pv_voltage, &data.pv_current, &data.pwm_duty, &data.radiator_temp, &data.mosfet_overheat_on, &data.airgap_temp);
                //comms_power.getc();
                serial.lock();
                data.pv_voltage = atof(pFields[0]);
                data.pv_current = atof(pFields[1]);
                data.pwm_duty = atof(pFields[2]);
                data.radiator_temp = atof(pFields[3]);
                data.mosfet_overheat_on = atoi(pFields[4]);
                data.airgap_temp = atof(pFields[5]);
                
                data.pv_power = data.pv_voltage * data.pv_current;
                serial.unlock();
            } else {
                comms_pc.printf("No Data available\r\n");
            }
            return NS_OK;
        } else {
            comms_pc.printf("[ERROR] RESTART\r\n");
            serial.lock();
            //send_cmd(CMD_POWER_BOARD_STOP);
            //comms_pc.printf("POWER BOARD stopped\r\n");
            while(comms_power.readable()) {
                comms_power.getc();
            }
            serial.unlock();
            return response;
        }
    }
    
    uint8_t get_calibration_data(void) {
        uint8_t value_count;
        uint8_t response = send_cmd(KEYBOARD_GET_CALIB_DATA);
        if(response == INCOMING_DATA) {
            serial.lock();
            receive_message();
            if (newData == true) {
                newData = false;
                strcpy(tempChars, receivedChars);
                value_count = parse_fields(receivedChars, pFields, NUM_FIELDS, ',');
                //comms_power.scanf("#%f,%f$", &data.pv_ref_voltage, &data.pv_ref_current);
                //comms_power.getc();
            }
            serial.unlock();
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
            comms_pc.printf("set PWM duty %f\r\n", data.pwm_duty);
            return NS_OK;
        } else {
            comms_pc.printf("Error: 0x%X\r\n", response);
        }
    }
}

void error_message(uint8_t error) {
    comms_pc.printf("[ERROR] %d\r\n", error);
    switch(error) {
        case SETUP_ERROR:
            
        break;
        
        case STARTUP_ERROR:
            
        break;
        
        case ADC_ERROR:        //can't find both ADC sensors
            comms_pc.printf("[ERROR] Can't read ADC sensors\r\n");
        break;
        
        case ADC_VOLTAGE_ERROR:
            comms_pc.printf("[ERROR] Can't read ADC voltage sensor\r\n");
        break;
        
        case ADC_CURRENT_ERROR:
            comms_pc.printf("[ERROR] Can't read ADC voltage sensor\r\n");
        break;
        
        case ADC_SETUP_ERROR:
            
        break;
        
        case FLASH_ACCESS_ERROR:
            comms_pc.printf("[ERROR] Can't access FLASH memory\r\n");
        break;
        
        case FLASH_READ_ERROR:
            comms_pc.printf("[ERROR] Can't read FLASH memory\r\n");
        break;
        
        case FLASH_WRITE_ERROR:
            comms_pc.printf("[ERROR] Can't write to FLASH memory\r\n");
        break;
        
        case CALIBRATION_ERROR:          //no calibration data
            comms_pc.printf("[ERROR] No CALIBRATION data\r\n");
        break;
        
        case DC_OVER_VOLTAGE:            //V_pv > 350V
            comms_pc.printf("[ERROR] DC VOLTAGE > 350V\r\n");
        break;
        
        case DC_OVER_CURRENT:            //I_pv > 10A
            comms_pc.printf("[ERROR] DC CURRENT > 10A\r\n");
        break;
        
        case DC_CURRENT_LEAKS:           //could be a faulty relay, or a short
            comms_pc.printf("[ERROR] DC CURRENT leaks. Could be faulty relay or a short circuit\r\n");
        break;
        
        case I2C_ERROR:
        break;
        
        case OVERHEAT:
            comms_pc.printf("[ERROR] DEVICE OVERHEAT\r\n");
        break;
        
        case RADIATOR_OVERHEAT:
            comms_pc.printf("[ERROR] Radiator TEMPERATURE > 70C\r\n");
        break;
        
        case AIRGAP_OVERHEAT:
            comms_pc.printf("[ERROR] Airgap TEMPERATURE > 70C\r\n");
        break;
        
        default:
        break;
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

// *******************************Nectar Sun Copyright � Nectar Sun 2017*************************************   
