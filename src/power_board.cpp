#include "mbed.h"
#include "data.h"
#include "power_board.h"
#include "OperationalMode.h"
#include "error_handler.h"

#include "NectarContract.h"

bool isFirst = true;

namespace power_board {
  Ticker get_data_tick;
  Timeout line_busy_timeout;
  Timeout error_timeout;

  static const PinName TX = PB_10;
  static const PinName RX = PB_11;
  
  volatile bool error_clearing = false;
  static uint8_t error_counter = 0;
  
  powerStream m_stream(TX, RX);
  
  void get_data_ISR() {
    nectar_contract::MainBoardStateForPower mainStateForPower = {
      data.grid_kwh,
      data.start,
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
    data.start = true;
  }
  
  void stop() {
    data.start = false;
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
  static const float interval = 1.0;
  
  m_serial.baud(C_SERIAL_BAUD_RATE);
  power_board::get_data_tick.attach(&power_board::get_data_ISR, interval);
  
  m_serial.attach(callback(this, &powerStream::Rx_interrupt));
  printf("power_serial setup\r\n");
}

void powerStream::Rx_interrupt() {
  while(m_serial.readable()) {
    __disable_irq();
    char rcv = m_serial.getc();
    //printf("%c", rcv);
    stream.receiveChar(rcv);
    __enable_irq();
  }
}

void powerStream::write(uint8_t byte) {
  //while(!m_serial.writeable()) {}
  m_serial.putc(byte);
}

void powerStream::received_power_board_state(const nectar_contract::PowerBoardState &state) {
  data.pv_power = state.sun_power;
  data.pv_voltage = state.sun_voltage;
  data.pv_current = state.sun_current;
  data.pwm_duty = state.pwm_duty;
  data.mosfet_overheat_on = state.transistor_overheat_on;
  PowerBoardError.save_error_code(state.power_board_error_code);
  data.calibrated = state.device_calibrated;
  data.generator_on = state.pwm_generator_on;
  data.solar_kwh = state.sun_meter_kwh;
  data.pv_ref_voltage = state.ref_voltage;
  data.pv_ref_current = state.ref_current;
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
