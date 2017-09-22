#include "mbed.h"
#include "device_modes.h"
#include "data.h"
#include <string>
#include "main_board.h"
#include "NectarStream.h"
#include "NectarContract.h"

namespace main_board {
  static const PinName TX = PC_10;
  static const PinName RX = PC_11;
  
  mbedStream m_stream(TX, RX);

  void send_stats() {
    nectar_contract::PowerBoardStats stats = {
      /*
      float sun_voltage;
      float sun_current;
      float pwm_duty;
      float radiator_temperature;
      bool transistor_overheat_on;
      float airgap_temperature;
      uint8_t power_board_error_code;
      uint8_t device_calibrated;
      uint8_t pwm_generator_on;
      */
      
      data.moment_voltage, data.moment_current, data.pwm_duty, data.radiator_temperature, data.mosfet_overheat_on,
      data.airgap_temperature, data.error, data.calibrated, data.generator_on
    };
    
    m_stream.stream.sendObject(C_POWER_BOARD_STATS, &stats, sizeof(stats));
    printf("sent %d bytes to main board\r\n", sizeof(stats));
  }
  
  void setup() {
    m_stream.setup();
  }
}

/*
 * Class implementation
 */

void mbedStream::setup() {
  m_serial.baud(C_SERIAL_BAUD_RATE);
  printf("m_serial setup\r\n");
  m_serial.attach(this, &mbedStream::Rx_interrupt);
}

void mbedStream::Rx_interrupt() {
  while(m_serial.readable()) {
    char rcv = m_serial.getc();
    printf("%c", rcv);
    stream.receiveChar(rcv);
  }
}

void mbedStream::write(uint8_t byte) {
  m_serial.putc(byte);
}

void mbedStream::received_cmd_stats() {
  main_board::send_stats();
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   

//  
//  void loop() {
//    if(command != NULL) {
//      
//      printf("cmd from main board: 0x%X\r\n", command);
//      switch(command) {
//        case POWER_BOARD_START:
//          if(data.error != 0x00) {
//            main_board_serial.putc(NACK);
//            main_board_serial.putc(data.error);
//          } else {
//            main_board_serial.putc(ACK);
//            device_modes::start();
//          }
//        break;
//        
//        case POWER_BOARD_STOP:
//          main_board_serial.putc(ACK);
//          device_modes::stop();
//        break;
//        
//        case CLEAR_ERROR:
//          main_board_serial.putc(ACK);
//          data.error = 0x00;
//        break;
//        
//        case ENTER_SERVICE_MODE:
//          main_board_serial.putc(ACK);
//          device_modes::enter_service_mode();
//        break;
//        
//        case INCREASE_PWM:
//          main_board_serial.putc(ACK);
//          device_modes::increase_pwm_duty();
//        break;
//        
//        case DECREASE_PWM:
//          main_board_serial.putc(ACK);
//          device_modes::decrease_pwm_duty();
//        break;
//        
//        case GET_DATA:
//          main_board_serial.putc(ACK);
//          main_board_serial.printf("#%.2f,%.2f,%.2f,%.2f,%d,%.2f,%d,%d,%d$\n",
//            data.moment_voltage, data.moment_current, data.pwm_duty, data.radiator_temperature,
//            data.mosfet_overheat_on, data.airgap_temperature, data.error, data.calibrated, data.generator_on
//          );
//          printf("[SENT] %f, %f, %f, %f, %d, %f, 0x%X\r\n", data.moment_voltage, data.moment_current, data.pwm_duty, data.radiator_temperature, data.mosfet_overheat_on, data.airgap_temperature, data.error);
//        break;
//        
//        case GET_REF_DATA:
//          main_board_serial.putc(ACK);
//          main_board_serial.printf("#&%f,%f$\n",
//            data.reference_voltage, data.reference_current
//          );
//          printf("[SENT] %f, %f\r\n", data.reference_voltage, data.reference_current);
//        break;

//        default:
//          main_board_serial.putc(NACK);
//          main_board_serial.putc(BAD_CMD);
//          pc.printf("Unrecognized CMD 0x%X\r\n", command);
//        break;
//      }
//    }
//  }

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
