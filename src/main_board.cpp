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
    //printf("sending %.3f, %.2f, %.2f, %d, %d\r\n", data.moment_voltage, data.moment_current, data.pwm_duty, data.calibrated, data.generator_on);
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
    //printf("sent %d bytes to main board\r\n", sizeof(stats));
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
    __disable_irq();
    char rcv = m_serial.getc();
    //printf("%c", rcv);
    stream.receiveChar(rcv);
    __enable_irq();
  }
}

void mbedStream::write(uint8_t byte) {
  m_serial.putc(byte);
}

void mbedStream::received_cmd_stats() {
  main_board::send_stats();
}

void mbedStream::received_cmd_power_start() {
  device_modes::start();
}

void mbedStream::received_cmd_power_stop() {
  device_modes::stop();
}

void mbedStream::received_cmd_service_clear_error() {
  data.error = 0x00;
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
