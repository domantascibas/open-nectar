#include "device_modes.h"
#include "ErrorHandler.h"
#include "data.h"
#include "main_board.h"

namespace main_board {
  static const PinName TX = PC_10;
  static const PinName RX = PC_11;
  
  mbedStream m_stream(TX, RX);
  
  bool isFirst;
  
  void setup() {
    m_stream.setup();
    isFirst = true;
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
//    printf("%c", rcv);
    stream.receiveChar(rcv);
    __enable_irq();
  }
}

void mbedStream::write(uint8_t byte) {
  if(!data.isCalibrating) {
  //  printf("%c", byte);
    m_serial.putc(byte);
  }
}

void mbedStream::received_main_board_state_for_power(const nectar_contract::MainBoardStateForPower &state) {
  if(main_board::isFirst) {
    main_board::isFirst = false;
  } else {
    data.grid_energy_meter_kwh = state.grid_meter_kwh;
  }
  data.isInOnboarding = state.is_in_onboarding;
  data.isTestMode = state.is_test_mode_on;
  if(state.start) {
    device_modes::start();
  } else {
    device_modes::stop();
  }
  printf("MAIN -> %f %d %d %d\r\n", data.grid_energy_meter_kwh, data.current_state, data.isTestMode, data.isInOnboarding);
}

StreamObject mbedStream::get_power_board_state() {
  powerState.sun_power = data.moment_power;
  powerState.sun_voltage = data.moment_voltage;
  powerState.sun_current = data.moment_current;
  powerState.pwm_duty = mppt.get_duty();
  powerState.device_temperature = data.device_temperature;
  powerState.transistor_overheat_on = data.mosfet_overheat_on;
  powerState.power_board_error_code = nectarError.get_errors();
  powerState.device_calibrated = !nectarError.has_error(CALIBRATION_ERROR);
  powerState.pwm_generator_on = mppt.is_generator_on();
  powerState.sun_meter_kwh = data.sun_energy_meter_kwh;
  powerState.grid_meter_kwh = data.grid_energy_meter_kwh;
  powerState.ref_voltage = sensors.get_voltage_reference();
  powerState.ref_current = sensors.get_current_reference();
  
  StreamObject _powerState(&powerState, sizeof(powerState));
  printf("-> MAIN %f %f %f %f %f %d %d %d %d %f %f %f %f\r\n",
    powerState.sun_power,
    powerState.sun_voltage,
    powerState.sun_current,
    powerState.pwm_duty,
    powerState.device_temperature,
    powerState.transistor_overheat_on,
    powerState.power_board_error_code,
    powerState.device_calibrated,
    powerState.pwm_generator_on,
    powerState.sun_meter_kwh,
    powerState.grid_meter_kwh,
    powerState.ref_voltage,
    powerState.ref_current);
  return _powerState;
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
