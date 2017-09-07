#include "mbed.h"
#include "data.h"
#include "esp.h"
#include "NectarStream.h"
#include "NectarContract.h"

static const PinName TX = PA_9;
static const PinName RX = PA_10;

static const uint8_t SET_MODE = 0x4D;       //'M'
static const uint8_t GET_STATS = 0x54;      //'T'

mbedStream m_stream(TX, RX);

namespace esp {
  static volatile char command = NULL;
  
  void send_stats() {    
    nectar_contract::MainBoardStats stats = {
      data.pv_power, data.grid_relay_on, data.temp_boiler, data.sun_relay_on,
      data.pv_voltage, data.pv_current, data.device_temperature, data.mosfet_overheat_on,
      data.radiator_temp, data.pwm_duty, data.airgap_temp, data.error, 0
    };
    
    m_stream.stream.sendObject(C_MAIN_BOARD_STATS, &stats, sizeof(stats));
  }

  void setup() {
    m_stream.setup();
  }

  void loop() {
  }
}

/*
 * Class implementation
 */

void mbedStream::setup() {
  m_serial.baud(19200);
  m_serial.attach(this, &mbedStream::Rx_interrupt);
}

void mbedStream::Rx_interrupt() {
  stream.receiveChar(m_serial.getc());
}

void mbedStream::write(uint8_t byte) { m_serial.putc(byte); }

void mbedStream::received_main_board_stats(const nectar_contract::MainBoardStats &stats) {
  esp::send_stats();
}

void mbedStream::received_esp_status(const nectar_contract::NectarStatus &status) {}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
