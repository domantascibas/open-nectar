#include "data.h"
#include "PowerBoardComms.h"
#include "OperationalMode.h"
#include "ErrorHandler.h"

namespace power_board {
  Ticker get_data_tick;
  Timeout line_busy_timeout;
  Timeout error_timeout;
  bool isFirst = true;

  static const PinName TX = PB_10;
  static const PinName RX = PB_11;
  
  volatile bool error_clearing = false;
  static uint8_t error_counter = 0;
  static bool isPowerBoardStarted = false;
  
  powerStream m_stream(TX, RX);
  
  float sun_power = 0.0;
  float sun_voltage = 0.0;
  float sun_current = 0.0;
  float pwm_duty = 0.1;
  
  uint8_t mosfet_overheat_on = false;
  uint32_t power_board_error = 0;

  bool calibrated = false;
  bool generator_on = false;
  float solar_kwh = 0.0;
  float grid_kwh = 0.0;
  float ref_voltage = 0.0;
  float ref_current = 0.0;
    
  nectar_contract::PowerBoardState powerBoardData = {
    sun_power,
    sun_voltage,
    sun_current,
    pwm_duty,
    mosfet_overheat_on,
    power_board_error,
    calibrated,
    generator_on,
    solar_kwh,
    grid_kwh,
    ref_voltage,
    ref_current
  };
  
  void get_data_ISR() {
    nectar_contract::MainBoardStateForPower mainStateForPower = {
      powerBoardData.grid_meter_kwh,
      isPowerBoardStarted,
      deviceOpMode.inTestMode
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
    isPowerBoardStarted = true;
  }
  
  void stop() {
    isPowerBoardStarted = false;
  }
  
  void setup() {
    m_stream.setup();
  }
  
  void enter_test_mode() {
    deviceOpMode.setTestMode();
  }
  
  void loop() {
//    if((data.power_board_error != 0x00) & (!error_clearing)) {
//      if(error_counter < 5) {
//        error_clearing = true;
//        print_error(data.power_board_error);
//        printf("ERROR COUNTER ++\r\n");
//        error_timeout.attach(&error_timeout_handler, 10.0);
//        error_counter++;
//      } else {
//        m_stream.stream.sendObject(C_POWER_BOARD_STOP);
//        data.power_board_error = RESTART_REQUIRED;
//      }
//    } else if((data.power_board_error == 0x00) & (error_counter != 0)) {
//      error_counter = 0;
//      printf("ERROR COUNTER RESET\r\n");
//    } 
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
  power_board::powerBoardData = state;
  powerBoardError.save_error_code(state.power_board_error_code);
  
  if(power_board::isFirst) {
    power_board::isFirst = false;
    data.d_kwh = power_board::powerBoardData.sun_meter_kwh;
  }
  data.solar_kwh_today = power_board::powerBoardData.sun_meter_kwh - data.d_kwh;
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
