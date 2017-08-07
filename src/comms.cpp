#include "mbed.h"
#include "data.h"
#include "comms.h"
#include <string>

namespace comms {
  uint8_t parse_fields(char* inputBuffer, char** pFields, uint32_t numFields, char delimiterChars) {
    char* pString = inputBuffer;
    char* pField;
    uint8_t length = 0;
    for(uint8_t i=0; i<numFields; i++) {
      pField = strtok(pString, &delimiterChars);
      if(pField != NULL) {
        pFields[i] = pField;
        length++;
      } else {
        pFields[i] = NULL;
      }
      pString = NULL;
    }
    return length;
  }

  void print_error(uint8_t error) {
    printf("[ERROR] %d\r\n", error);
    switch(error) {
      case SETUP_ERROR:

      break;

      case STARTUP_ERROR:

      break;

      case ADC_ERROR:        //can't find both ADC sensors
        printf("[ERROR] Can't read ADC sensors\r\n");
      break;

      case ADC_VOLTAGE_ERROR:
        printf("[ERROR] Can't read ADC voltage sensor\r\n");
      break;

      case ADC_CURRENT_ERROR:
        printf("[ERROR] Can't read ADC voltage sensor\r\n");
      break;

      case ADC_SETUP_ERROR:

      break;

      case FLASH_ACCESS_ERROR:
        printf("[ERROR] Can't access FLASH memory\r\n");
      break;

      case FLASH_READ_ERROR:
        printf("[ERROR] Can't read FLASH memory\r\n");
      break;

      case FLASH_WRITE_ERROR:
        printf("[ERROR] Can't write to FLASH memory\r\n");
      break;

      case CALIBRATION_ERROR:          //no calibration data
        printf("[ERROR] No CALIBRATION data\r\n");
      break;

      case DC_OVER_VOLTAGE:            //V_pv > 350V
        printf("[ERROR] DC VOLTAGE > 350V\r\n");
      break;

      case DC_OVER_CURRENT:            //I_pv > 10A
        printf("[ERROR] DC CURRENT > 10A\r\n");
      break;

      case DC_CURRENT_LEAKS:           //could be a faulty relay, or a short
        printf("[ERROR] DC CURRENT leaks. Could be faulty relay or a short circuit\r\n");
      break;

      case I2C_ERROR:
      break;

      case OVERHEAT:
        printf("[ERROR] DEVICE OVERHEAT\r\n");
      break;

      case RADIATOR_OVERHEAT:
        printf("[ERROR] Radiator TEMPERATURE > 70C\r\n");
      break;

      case AIRGAP_OVERHEAT:
        printf("[ERROR] Airgap TEMPERATURE > 70C\r\n");
      break;

      default:
      break;
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
