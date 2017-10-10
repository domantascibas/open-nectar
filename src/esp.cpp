#include "mbed.h"
#include "data.h"
#include "esp.h"

#include "NectarStream.h"
#include "NectarContract.h"

namespace esp {
  static const PinName TX = PA_9;   //D8
  static const PinName RX = PA_10;  //D2
  
  mbedStream m_stream(TX, RX);
  
  void send_stats() {
    nectar_contract::MainBoardStats stats = {
      data.pv_power, data.grid_relay_on, data.temp_boiler, data.sun_relay_on,
      data.pv_voltage, data.pv_current, data.device_temperature, data.mosfet_overheat_on,
      data.radiator_temp, data.pwm_duty, data.error, 0, data.solar_kwh, data.grid_kwh
    };
    
    m_stream.stream.sendObject(C_MAIN_BOARD_STATS, &stats, sizeof(stats));
    printf("sent %d bytes\r\n", sizeof(stats));
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
  printf("esp_serial setup\r\n");
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
  esp::send_stats();
}

void mbedStream::received_time(const nectar_contract::NectarTime &time) {
  set_time(time.time);
  
  char buffer[32];
  time_t got_time = time.time;
  
  strftime(buffer, 32, "%Y/%m/%d %I:%M:%S\r\n", localtime(&got_time));
  printf("time %s", buffer);
}

void mbedStream::received_status(const nectar_contract::NectarStatus &status) {
  EspDeviceData = status;
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
