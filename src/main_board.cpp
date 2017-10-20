#include "mbed.h"
#include "device_modes.h"
#include "error_handler.h"
#include "data.h"
#include "main_board.h"
#include "NectarStream.h"
#include "NectarContract.h"

namespace main_board {
  static const PinName TX = PC_10;
  static const PinName RX = PC_11;
  
  mbedStream m_stream(TX, RX);
  
  void setup() {
    m_stream.setup();
  }
}

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

void mbedStream::received_main_board_state_for_power(const nectar_contract::MainBoardStateForPower &state) {
  data.grid_energy_meter_kwh = state.grid_meter_kwh;
  data.isTestMode = state.is_test_mode_on;
  if(state.start) {
    device_modes::start();
  } else {
    device_modes::stop();
  }
}

StreamObject mbedStream::get_power_board_state() {
  nectar_contract::PowerBoardState powerState = {
    data.moment_power,
    data.moment_voltage,
    data.moment_current,
    data.pwm_duty,
    data.mosfet_overheat_on,
    NectarError.get_errors(),
    data.calibrated,
    data.generator_on,
    data.sun_energy_meter_kwh,
    data.reference_voltage,
    data.reference_current
  };
  
  StreamObject _powerState(&powerState, sizeof(powerState));
  return _powerState;
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
