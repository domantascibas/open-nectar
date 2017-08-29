#include "mbed.h"
#include "device_modes.h"
#include "data.h"
#include <string>
#include "main_board.h"

static const PinName PC_TX = PA_2;
static const PinName PC_RX = PA_3;
static const PinName TX = PC_10;
static const PinName RX = PC_11;

static const uint8_t POWER_BOARD_STOP = 0x30;       //'0'
static const uint8_t POWER_BOARD_START = 0x31;      //'1'
static const uint8_t GET_DATA = 0x32;               //'2'
static const uint8_t GET_REF_DATA = 0x33;           //'3'
static const uint8_t ENTER_SERVICE_MODE = 0x6D;     //'m'

static const uint8_t INCREASE_PWM = 0x2B;           //'+'
static const uint8_t DECREASE_PWM = 0x2D;           //'-'
static const uint8_t CLEAR_ERROR = 0x20;            //'SPACE'

RawSerial pc(PC_TX, PC_RX);
RawSerial main_board_serial(TX, RX);

namespace main_board {
  static volatile char command = NULL;

  void Rx_interrupt() {
    command = main_board_serial.getc();
    main_board_serial.putc(command);
  }
  
  void setup() {
    static const uint32_t pc_baud = 115200;
    static const uint32_t main_baud = 19200;
    
    pc.baud(pc_baud);
    printf("\r\n[START]\r\n");
    printf("[COMMS PC]\r\n");
    printf("[ok] baud %d\r\n\n", pc_baud);
    printf("[COMMS MAIN BOARD]\r\n");
    main_board_serial.baud(main_baud);
    main_board_serial.attach(&Rx_interrupt);
    printf("[ok] baud %d\r\n\n", main_baud);
  }
  
  void loop() {
    if(command != NULL) {
      
      printf("cmd from main board: 0x%X\r\n", command);
      switch(command) {
        case POWER_BOARD_START:
          if(data.error != 0x00) {
            main_board_serial.putc(NACK);
            main_board_serial.putc(data.error);
          } else {
            main_board_serial.putc(ACK);
            device_modes::start();
          }
        break;
        
        case POWER_BOARD_STOP:
          main_board_serial.putc(ACK);
          device_modes::stop();
        break;
        
        case CLEAR_ERROR:
          main_board_serial.putc(ACK);
          data.error = 0x00;
        break;
        
        case ENTER_SERVICE_MODE:
          main_board_serial.putc(ACK);
          device_modes::enter_service_mode();
        break;
        
        case INCREASE_PWM:
          main_board_serial.putc(ACK);
          device_modes::increase_pwm_duty();
        break;
        
        case DECREASE_PWM:
          main_board_serial.putc(ACK);
          device_modes::decrease_pwm_duty();
        break;
        
        case GET_DATA:
          main_board_serial.putc(ACK);
          main_board_serial.printf("#%.2f,%.2f,%.2f,%.2f,%d,%.2f,%d,%d,%d$\n",
            data.moment_voltage, data.moment_current, data.pwm_duty, data.radiator_temperature,
            data.mosfet_overheat_on, data.airgap_temperature, data.error, data.calibrated, data.generator_on
          );
          printf("[SENT] %f, %f, %f, %f, %d, %f, 0x%X\r\n", data.moment_voltage, data.moment_current, data.pwm_duty, data.radiator_temperature, data.mosfet_overheat_on, data.airgap_temperature, data.error);
        break;
        
        case GET_REF_DATA:
          main_board_serial.putc(ACK);
          main_board_serial.printf("#&%f,%f$\n",
            data.reference_voltage, data.reference_current
          );
          printf("[SENT] %f, %f\r\n", data.reference_voltage, data.reference_current);
        break;

        default:
          main_board_serial.putc(NACK);
          main_board_serial.putc(BAD_CMD);
          pc.printf("Unrecognized CMD 0x%X\r\n", command);
        break;
      }
      command = NULL;
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
