#include "mbed.h"
#include "utils.h"
#include "data.h"
#include "esp_comms.h"
#include <string>
#include <sstream>
#include <iostream>

Serial comms_esp(ESP_TX, ESP_RX);
extern data_collection nectar_data;

data_collection nectar_data = {80.0, 18.0, 56.0, 42.0, 0, 0, 0, 0.1, 23.3, 32.0, false, false, false, false, MODE_DEFAULT};

namespace esp {
    char* pFields[NUM_FIELDS];
    char buffer[128];
    
    // This function seperates the single input string into numFields substrings
    uint8_t parse_fields(char* inputBuffer, char** pFields, uint32_t numFields, char delimiterChars) {
        char* pString = inputBuffer;
        //printf("[ PARSING ] %s\n\r", inputBuffer);
        char* pField;
        uint8_t length = 0;        
        
        for(uint32_t i=0; i<numFields; i++) {
            pField = strtok(pString, &delimiterChars);
     
            if(pField != NULL) {
                pFields[i] = pField;
                length++;
            } else {
                pFields[i] = NULL;
            }
            pString = NULL; //to make strtok continue parsing the next field rather than start again on the original string (see strtok documentation for more details)
        }
        return length;
    }
    
    uint8_t set_mode(void) {
        comms_esp.scanf("%s", buffer); //read from ESP serial
        comms_esp.getc();
        printf("[ RECEIVED] %s\n\r", buffer);
        uint8_t value_count;
        value_count = parse_fields(buffer, pFields, NUM_FIELDS, ',');
        //printf("[ READ ] %d\n\r", value_count);
        
        nectar_data.temperature_max = atof(pFields[0]);
        nectar_data.temperature_min = atof(pFields[1]);
        nectar_data.temperature_scheduled = atof(pFields[2]);
        nectar_data.current_mode = atoi(pFields[3]);
        
        //if(nectar_data.temperature_max < 1.0) {
        //    printf("[ ERROR ] %s %f %s %f %s %f %s %d\n\r", pFields[0], atof(pFields[0]), pFields[1], atof(pFields[1]), pFields[2], atof(pFields[2]), pFields[3], atoi(pFields[3]));
        //} else {
        //    printf("[ RECEIVED ] %f %f %f %d\n\r", nectar_data.temperature_max, nectar_data.temperature_min, nectar_data.temperature_scheduled, nectar_data.current_mode);
        //}
        
        return 0;
    }
    
    uint8_t get_stats() {
        int length;
        ostringstream message;
        message << nectar_data.temperature_moment << ","
                << nectar_data.sun_power_moment << ","
                << nectar_data.sun_voltage << ","
                << nectar_data.sun_current << ","
                << nectar_data.pwm_duty << ","
                << nectar_data.device_temperature << ","
                << nectar_data.capacitor_temperature << ","
                << nectar_data.sun_relay_on << ","
                << nectar_data.grid_relay_on << ","
                << nectar_data.transistor_overheat_on
        ;
        
        string s = message.str();
        const char* p = s.c_str();
        length = strlcpy(buffer, p, sizeof(buffer) - 1);
        
        printf("[ SENT ] %s\n\r", buffer);
        comms_esp.printf("%s\n", buffer); //print to ESP serial
        return 0; //returns operation mode
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
