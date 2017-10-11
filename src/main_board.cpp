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
    printf("sending %.3f, %.2f, %.2f, %d, %d\r\n", data.moment_voltage, data.moment_current, data.pwm_duty, data.calibrated, data.generator_on);
    nectar_contract::PowerBoardStats stats = {
      data.moment_power, data.moment_voltage, data.moment_current, data.pwm_duty, data.radiator_temperature, data.mosfet_overheat_on,
      data.error, data.calibrated, data.generator_on, data.sun_energy_meter_kwh
    };
    
    m_stream.stream.sendObject(C_POWER_BOARD_STATS, &stats, sizeof(stats));
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
  m_serial.attach(callback(this, &mbedStream::Rx_interrupt));
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
  printf("[COMMAND] start\r\n");
}

void mbedStream::received_cmd_power_stop() {
  device_modes::stop();
  printf("[COMMAND] stop\r\n");
}

void mbedStream::received_cmd_service_clear_error() {
  data.error = 0x00;
}

void mbedStream::received_grid_meter(const nectar_contract::PowerBoardGridMeter &grid_meter) {
  data.grid_energy_meter_kwh = grid_meter.grid_meter_kwh;
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
