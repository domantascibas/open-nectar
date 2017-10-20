#include "mbed.h"
#include "data.h"
#include "esp.h"
#include "error_handler.h"

#include "NectarContract.h"

namespace esp {
  Ticker get_data_tick;

  static const PinName TX = PA_9;   //D8
  static const PinName RX = PA_10;  //D2
  
  mbedStream m_stream(TX, RX);
  
  void get_data_ISR() {
    nectar_contract::MainBoardStateForESP mainStateForEsp = {
      data.pv_power,
      data.grid_relay_on,
      data.temp_boiler,
      data.sun_relay_on,
      data.pv_voltage,
      data.pv_current,
      data.device_temperature,
      data.mosfet_overheat_on,
      data.pwm_duty,
      PowerBoardError.get_errors(),
      MainBoardError.get_errors(),
      data.solar_kwh,
      data.grid_kwh,
      false,
      false,
      false
    };
    
    StreamObject _mainStateForEsp(&mainStateForEsp, sizeof(mainStateForEsp));
    m_stream.stream.send_state_to_esp(_mainStateForEsp);
  }

  void setup() {
    m_stream.setup();
  }
}

/*
 * Class implementation
 */

void mbedStream::setup() {
  static const float interval = 15.0;
  
  m_serial.baud(C_SERIAL_BAUD_RATE);
  esp::get_data_tick.attach(&esp::get_data_ISR, interval);
  m_serial.attach(callback(this, &mbedStream::Rx_interrupt));
  printf("esp_serial setup\r\n");
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

void mbedStream::received_esp_state(const nectar_contract::ESPState &state) {
  EspDeviceData = state;
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
