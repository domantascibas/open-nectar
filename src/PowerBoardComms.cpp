#include "PowerBoardComms.h"
#include "ErrorHandler.h"
#include "DataService.h"
#include "EnergyMeter.h"

namespace power_board {
  Ticker get_data_tick;
  bool isFirst = true;

  static bool startPowerBoard = false;
  static const PinName TX = PB_10;
  static const PinName RX = PB_11;
  
  powerStream m_stream(TX, RX);
  
  void get_data_ISR() {
    nectar_contract::MainBoardStateForPower mainStateForPower = {
      gridMeter.getMeterReading(),
      startPowerBoard,
      deviceOpMode.isInTestMode()
    };
    
    StreamObject _mainStateForPower(&mainStateForPower, sizeof(mainStateForPower));
    m_stream.stream.send_state_to_power_board(_mainStateForPower);
//    printf("[OUT] %d bytes\r\n", sizeof(mainStateForPower));
//    printf("[OUT] %f %d %d\r\n",
//    mainStateForPower.grid_meter_kwh,
//    mainStateForPower.start,
//    mainStateForPower.is_test_mode_on);
  }
  
  void start() {
    startPowerBoard = true;
  }
  
  void stop() {
    startPowerBoard = false;
  }
  
  void setup() {
    m_stream.setup();
  }
}

/*
 * Class implementation
 */

void powerStream::setup() {
  static const float interval = 2.0;
  
  m_serial.baud(C_SERIAL_BAUD_RATE);
  power_board::get_data_tick.attach(&power_board::get_data_ISR, interval);
  
  m_serial.attach(callback(this, &powerStream::Rx_interrupt));
  printf("power_serial setup\r\n");
}

void powerStream::Rx_interrupt() {
  while(m_serial.readable()) {
    __disable_irq();
    char rcv = m_serial.getc();
//    printf("%c", rcv);
    stream.receiveChar(rcv);
    __enable_irq();
  }
}

void powerStream::write(uint8_t byte) {
//  printf("%c", byte);
  m_serial.putc(byte);
}

void powerStream::received_power_board_state(const nectar_contract::PowerBoardState &state) {
//  printf("[IN] %d bytes\r\n", sizeof(state));
  __disable_irq();
  powerData = state;
  powerBoardError.save_error_code(state.power_board_error_code);
  
  if(power_board::isFirst) gridMeter.setMeterReading(powerData.grid_meter_kwh);
  DataService::calculateSolarKwhDiff(power_board::isFirst);
  power_board::isFirst = false;
  __enable_irq();
//  printf("[IN] %f %f %f %f %d %d %d %d %f %f %f\r\n",
//    data.pv_power,
//    data.pv_voltage,
//    data.pv_current,
//    data.pwm_duty,
//    data.mosfet_overheat_on,
//    powerBoardError.get_errors(),
//    data.calibrated,
//    data.generator_on,
//    data.solar_kwh,
//    data.pv_ref_voltage,
//    data.pv_ref_current);
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
