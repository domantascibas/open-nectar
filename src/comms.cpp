#include "mbed.h"
#include "rtos.h"
#include "data.h"
#include "comms.h"
#include <string>

//Serial com pins
#define PC_TX                       PA_2
#define PC_RX                       PA_3
#define POWER_TX                    PB_10
#define POWER_RX                    PB_11
#define ESP_TX                      PA_9
#define ESP_RX                      PA_10

//comms
RawSerial comms_pc(PC_TX, PC_RX);
RawSerial comms_power(POWER_TX, POWER_RX);
RawSerial comms_esp(ESP_TX, ESP_RX);

extern DigitalOut relay_sun;
extern DigitalOut relay_grid;
extern Data data;

const uint8_t num_chars = 128;
char* pFields[NUM_FIELDS];

namespace comms {
    void error_message(uint8_t);

    uint8_t send_cmd_power(uint8_t command) {
        //TODO add timeout after sending command
        uint8_t response;
        comms_power.putc(command);
        response = comms_power.getc();
        if(response == NACK) {
            response = comms_power.getc();
            error_message(response);
        }
        return response;
    }

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

    uint8_t receive_message_power() {
        char received_chars[128];
        uint8_t message_count = 0;
        bool new_data = false;
        bool recv_in_progress = false;
        uint8_t ndx = 0;
        char start_marker = '#';
        char end_marker = '$';
        char rc;
        
        while(!new_data) {
            rc = comms_power.getc();
            comms_pc.putc(rc);

            if (recv_in_progress) {
                if (rc != end_marker) {
                    received_chars[ndx] = rc;
                    ndx++;
                    if (ndx >= num_chars) {
                        ndx = num_chars - 1;
                    }
                } else {
                    received_chars[ndx] = '\0';  // terminate the string
                    comms_power.getc();         //take the \n char from the serial bus
                    recv_in_progress = false;
                    ndx = 0;
                    new_data = true;
                    comms_pc.printf("\r\n");
                }
            } else if (rc == start_marker) {
                recv_in_progress = true;
            }
        }
        
        if(new_data) {
            new_data = false;
            message_count = parse_fields(received_chars, pFields, NUM_FIELDS, ',');
            
            switch(message_count) {
                case 6:
                    data.pv_voltage = atof(pFields[0]);
                    data.pv_current = atof(pFields[1]);
                    data.pwm_duty = atof(pFields[2]);
                    data.radiator_temp = atof(pFields[3]);
                    data.mosfet_overheat_on = atoi(pFields[4]);
                    data.airgap_temp = atof(pFields[5]);

                    data.pv_power = data.pv_voltage * data.pv_current;
                break;
                
                case 3:
                    //TODO receive calibration data
                break;
                
                default:
                    comms_pc.printf("[ERROR] partial message received\r\n");
                    return MSG_ERROR;
                break;
            }
        } else {
            comms_pc.printf("[WARNING] bad message\r\n");
        }
        
        return NS_OK;
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
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
