#include "consts.h"
#include "device_modes.h"
#include "power_controller.h"
#include "ErrorHandler.h"
#include "data.h"
#include "main_board_comms.h"
#include "sensor_controller.h"

mbedStream m_stream(NS_TX, NS_RX);

void main_board_comms_init(void) {
  m_stream.setup();
}

void mbedStream::setup() {
  m_serial.baud(C_SERIAL_BAUD_RATE);
  printf("m_serial setup\r\n");
  m_serial.attach(callback(this, &mbedStream::Rx_interrupt));
}

void mbedStream::Rx_interrupt() {
  data.safeToReadTemp = false;
  while(m_serial.readable()) {
    __disable_irq();
    char rcv = m_serial.getc();
    stream.receiveChar(rcv);
    __enable_irq();
  }
}

void mbedStream::write(uint8_t byte) {
  if(!data.isCalibrating) {
    m_serial.putc(byte);
  }
}

void mbedStream::received_main_board_state_for_power(const nectar_contract::MainBoardStateForPower &state) {
  if((state.grid_meter_kwh != 0) && (state.grid_meter_kwh > data.grid_energy_meter_kwh)) {
    data.grid_energy_meter_kwh = state.grid_meter_kwh;
  }

  data.isInOnboarding = (state.is_in_onboarding == 0xAC ? true : false);
  data.isTestMode = (state.is_test_mode_on == 0xAC ? true : false);
	data.startCalibration = (state.calibrate == 0xAC ? true : false);
	if(state.calibrate == 0xAC)
		data.isCalibrating = true;
	
  if(state.start == 0xAC) {
    device_modes_set_state_running();
  } else {
    device_modes_set_state_stop();
  }
  printf("MAIN -> %f %d %d %d %d %d\r\n", data.grid_energy_meter_kwh, data.current_state, data.isTestMode, data.isInOnboarding, data.startCalibration, (state.start == 0xAC ? true : false));
}

StreamObject mbedStream::get_power_board_state() {
  powerState.sun_power = data.moment_power;
  powerState.sun_voltage = data.moment_voltage;
  powerState.sun_current = data.moment_current;
  powerState.pwm_duty = power_controller_get_duty();
  powerState.device_temperature = data.device_temperature;
  powerState.transistor_overheat_on = (data.mosfet_overheat_on ? 0xAC : 0xFA);
  powerState.power_board_error_code = nectarError.get_errors();
  powerState.device_calibrated = (!nectarError.has_error(CALIBRATION_ERROR) ? 0xAC : 0xFA);
  powerState.pwm_generator_on = (power_controller_is_generator_on() ? 0xAC : 0xFA);
  powerState.sun_meter_kwh = data.sun_energy_meter_kwh;
  powerState.grid_meter_kwh = data.grid_energy_meter_kwh;
  powerState.ref_voltage = sensor_controller_get_voltage_ref();
  powerState.ref_current = sensor_controller_get_current_ref();
	powerState.power_version = NECTAR_POWER_BOARD_VERSION;
  
  StreamObject _powerState(&powerState, sizeof(powerState));
  printf("-> MAIN %f %f %f %f %f %d %d %d %d %f %f %f %f %1.2f\r\n",
    powerState.sun_power,
    powerState.sun_voltage,
    powerState.sun_current,
    powerState.pwm_duty,
    powerState.device_temperature,
    (powerState.transistor_overheat_on == 0xAC ? true : false),
    powerState.power_board_error_code,
    (powerState.device_calibrated == 0xAC ? true : false),
    (powerState.pwm_generator_on == 0xAC ? true : false),
    powerState.sun_meter_kwh,
    powerState.grid_meter_kwh,
    powerState.ref_voltage,
    powerState.ref_current,
		powerState.power_version);
  data.safeToReadTemp = true;
  return _powerState;
}
