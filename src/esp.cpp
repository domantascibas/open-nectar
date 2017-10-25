#include "mbed.h"
#include "data.h"
#include "esp.h"
#include "error_handler.h"
#include "menu_service.h"

#include "NectarContract.h"

namespace esp {
  Ticker get_data_tick;

  static const PinName TX = PA_9;   //D8
  static const PinName RX = PA_10;  //D2
  
  mbedStream m_stream(TX, RX);
  
  void get_data_ISR() {
    time_t rtc = time(NULL);
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
      time(NULL),
      false,
      false,
      false
    };
    
    StreamObject _mainStateForEsp(&mainStateForEsp, sizeof(mainStateForEsp));
    m_stream.stream.send_state_to_esp(_mainStateForEsp);
    printf("[OUT ESP] current time: %s\r\n", ctime(&rtc));
    printf("[OUT ESP] sent data to ESP %d, %d\r\n", sizeof(mainStateForEsp), sizeof(nectar_contract::MainBoardStateForESP));
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
//    printf("%c", rcv);
    stream.receiveChar(rcv);
    __enable_irq();
  }
}

void mbedStream::write(uint8_t byte) {
  m_serial.putc(byte);
}

void mbedStream::received_esp_state(const nectar_contract::ESPState &state) {
  espDeviceData = state;
  printf("[IN ESP] received %d %d %d %f %f %f %lld\r\n", espDeviceData.heater_mode, espDeviceData.is_configured, espDeviceData.has_internet_connection, espDeviceData.temperature, espDeviceData.temperature_max, espDeviceData.boiler_power, espDeviceData.sync_time);
  if(espDeviceData.sync_time != 0) {
    set_time(espDeviceData.sync_time);
    time_t sec = time(NULL);
    menu_actions::updateTime();
    printf("Time set from ESP: %s\r\n", ctime(&sec));
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
