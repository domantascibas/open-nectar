#include "data.h"
#include "EspComms.h"
#include "OperationalMode.h"
#include "ErrorHandler.h"
#include "PowerBoardComms.h"
#include "menu_service.h"

namespace esp {
  Ticker get_data_tick;

  static const PinName TX = PA_9;   //D8
  static const PinName RX = PA_10;  //D2
  
  mbedStream m_stream(TX, RX);
  
  nectar_contract::HeaterMode current_mode = nectar_contract::None;
  bool has_config = false;
  bool has_internet = false;
  
  float temp_max = 75.0;
  float temp_scheduled = 55.0;
  float boiler_power = 0.0;
  
  nectar_contract::ESPState espData = {
    current_mode,
    has_config,
    has_internet,
    temp_scheduled,
    temp_max,
    boiler_power,
    0
  };
  
  void get_data_ISR() {
    time_t rtc = time(NULL);
    nectar_contract::MainBoardStateForESP mainStateForEsp = {
      power_board::powerBoardData.sun_power,
      data.grid_relay_on,
      data.temp_boiler,
      data.sun_relay_on,
      power_board::powerBoardData.sun_voltage,
      power_board::powerBoardData.sun_current,
      data.device_temperature,
      power_board::powerBoardData.transistor_overheat_on,
      power_board::powerBoardData.pwm_duty,
      powerBoardError.get_errors(),
      mainBoardError.get_errors(),
      power_board::powerBoardData.sun_meter_kwh,
      data.grid_kwh,
      time(NULL),
      data.reset,
      deviceOpMode.isPairing,
      data.boost_off
    };
    
    StreamObject _mainStateForEsp(&mainStateForEsp, sizeof(mainStateForEsp));
    m_stream.stream.send_state_to_esp(_mainStateForEsp);
    data.reset = false;
    printf("[OUT ESP] current time: %s\r\n", ctime(&rtc));
    printf("[OUT ESP] sent data to ESP\r\n");
    printf("[OUT ESP] %d %d\r\n", deviceOpMode.isPairing, data.boost_off);
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
  esp::espData = state;
  if((nectar_contract::HeaterMode)esp::espData.heater_mode != nectar_contract::Boost) data.boost_off = false;
  printf("[IN ESP] received %d %d %d %f %f %f %lld\r\n", esp::espData.heater_mode, esp::espData.is_configured, esp::espData.has_internet_connection, esp::espData.temperature, esp::espData.temperature_max, esp::espData.boiler_power, esp::espData.sync_time);
  if(esp::espData.sync_time != 0) {
    set_time(esp::espData.sync_time);
    time_t sec = time(NULL);
    menu_actions::updateTime();
    printf("Time set from ESP: %s\r\n", ctime(&sec));
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
