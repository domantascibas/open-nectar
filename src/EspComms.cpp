#include "EspComms.h"
#include "device_modes.h"
#include "ErrorHandler.h"
#include "menu_service.h"
#include "DataService.h"

extern RelayController relayController;

namespace esp {
  Ticker get_data_tick;

  static const PinName TX = PA_9;   //D8
  static const PinName RX = PA_10;  //D2
  
  mbedStream m_stream(TX, RX);
  
  void get_data_ISR() {
    time_t rtc = time(NULL);
    nectar_contract::MainBoardStateForESP mainStateForEsp = {
      powerData.sun_power,
      relayController.isGridRelayOn(),
      temperatureData.getBoilerTemperature(),
      relayController.isSunRelayOn(),
      powerData.sun_voltage,
      powerData.sun_current,
      temperatureData.getDeviceTemperature(),
      powerData.transistor_overheat_on,
      powerData.pwm_duty,
      powerBoardError.get_errors(),
      mainBoardError.get_errors(),
      powerData.sun_meter_kwh,
      powerData.grid_meter_kwh,
      rtc,
      deviceOpMode.isReset(),
      deviceOpMode.isPairing(),
      deviceOpMode.isBoostOff()
    };
    
    StreamObject _mainStateForEsp(&mainStateForEsp, sizeof(mainStateForEsp));
    m_stream.stream.send_state_to_esp(_mainStateForEsp);
    printf("\r\nCurrent time: %s\r", ctime(&rtc));
    printf("-> ESP %f %f %f %f %d %d %f %f %d %d %d %f %f %d %d %d\r\n",
      mainStateForEsp.moment_sun_watt,
      mainStateForEsp.sun_voltage,
      mainStateForEsp.sun_current,
      mainStateForEsp.pwm_duty,
      mainStateForEsp.sun_relay_on,
      mainStateForEsp.grid_relay_on,
      mainStateForEsp.water_temperature,
      mainStateForEsp.device_temperature,
      mainStateForEsp.transistor_overheat_on,
      mainStateForEsp.main_board_error_code,
      mainStateForEsp.power_board_error_code,
      mainStateForEsp.sun_meter_kwh,
      mainStateForEsp.grid_meter_kwh,
      mainStateForEsp.reset,
      mainStateForEsp.pair_mode,
      mainStateForEsp.send_boost_off
      );
    if(deviceOpMode.isReset()) deviceOpMode.setReset(false);
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
  printf("[ESP] start\r\n");
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
  espData.pin = state.pin;
  if(state.is_configured) {
    espData = state;
    if((nectar_contract::HeaterMode)espData.heater_mode != nectar_contract::Boost)
      deviceOpMode.setBoostOff(false);
    if(DataService::getCurrentHeaterMode() != (nectar_contract::HeaterMode)espData.heater_mode) {
      printf("HEATER MODE UPDATED\r\n");
      DataService::setCurrentHeaterMode((nectar_contract::HeaterMode)espData.heater_mode);
    }
    device_modes::updateHeaterMode = true;
    printf("ESP -> %d %d %d %f %f %f %lld %d\r\n", espData.heater_mode, espData.is_configured, espData.has_internet_connection, espData.temperature, espData.temperature_max, espData.boiler_power, espData.sync_time, espData.pin);
    if(espData.sync_time != 0) {
      set_time(espData.sync_time);
      time_t sec = time(NULL);
      menu_actions::updateTime();
      printf("Time set from ESP: %s\r\n", ctime(&sec));
    }
  } else {
    printf("[ESP] NO CONFIG %d %d %d %f %f %f %lld %d\r\n", espData.heater_mode, espData.is_configured, espData.has_internet_connection, espData.temperature, espData.temperature_max, espData.boiler_power, espData.sync_time, espData.pin);
  }
}
