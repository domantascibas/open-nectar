#include "consts.h"
#include "pins.h"
#include "device_modes.h"
#include "power_controller.h"
#include "error_controller.h"
// #include "data.h"
#include "main_board_comms.h"
#include "sensor_controller.h"

extern "C" {
    #include "data.h"
}

nectar_contract::PowerBoardState powerState = {
  0,
  0,
  0,
  0.1f,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0.0f,
  0.0f,
  0.0f
};

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
  data_setSafeToReadTemp(0);
  while(m_serial.readable()) {
    __disable_irq();
    char rcv = m_serial.getc();
    stream.receiveChar(rcv);
    __enable_irq();
  }
}

void mbedStream::write(uint8_t byte) {
  if(!data_getIsCalibrating()) {
    m_serial.putc(byte);
  }
}

void mbedStream::received_main_board_state_for_power(const nectar_contract::MainBoardStateForPower &state) {
  uint32_t g;
  PowerData_read(GRID_METER, &g);
  if((state.grid_meter_kwh != 0) && (METER_CONVERT(state.grid_meter_kwh) > g)) {
    // data.grid_energy_meter_kwh = state.grid_meter_kwh;
    g = METER_CONVERT(state.grid_meter_kwh);
    PowerData_write(GRID_METER, &g);
  }

  data_setIsInOnboarding(state.is_in_onboarding == COMMS_TRUE_VALUE ? true : false);
  data_setIsTestMode(state.is_test_mode_on == COMMS_TRUE_VALUE ? true : false);
	data_setStartCalibration(state.calibrate == COMMS_TRUE_VALUE ? true : false);
	if(state.calibrate == COMMS_TRUE_VALUE) {
		data_setIsCalibrating(1);
  }
	
  if(state.start == COMMS_TRUE_VALUE) {
    device_modes_set_state_running();
  } else {
    device_modes_set_state_stop();
  }
  if(data_getIsTestMode()) {
    printf("[TEST MODE]\r\n");
  }
  // printf("MAIN -> %f %d %d %d %d %d\r\n", data.grid_energy_meter_kwh, data.current_state, data.isTestMode, data.isInOnboarding, data.startCalibration, (state.start == COMMS_TRUE_VALUE ? true : false));
}

StreamObject mbedStream::get_power_board_state() {
  uint8_t t;
  uint16_t v, v_ref, i_ref;
  uint32_t p;
  PowerData_read(M_POWER, &p);
  powerState.sun_power = DIV_POWER_CONVERT(p);
  PowerData_read(M_VOLTAGE, &v);
  powerState.sun_voltage = DIV_VI_CONVERT(v);
  PowerData_read(M_CURRENT, &v);
  powerState.sun_current = DIV_VI_CONVERT(v);
  powerState.pwm_duty = power_controller_get_duty();
  PowerData_read(TEMPERATURE, &t);
  powerState.device_temperature = DIV_T_CONVERT(t);
  powerState.transistor_overheat_on = (GET_STATUS(OVERHEAT_STATUS) ? COMMS_TRUE_VALUE : COMMS_FALSE_VALUE);
  powerState.power_board_error_code = nectarError_get_errors();
  powerState.device_calibrated = (!nectarError_has_error(CALIBRATION_ERROR) ? COMMS_TRUE_VALUE : COMMS_FALSE_VALUE);
  powerState.pwm_generator_on = (power_controller_is_generator_on() ? COMMS_TRUE_VALUE : COMMS_FALSE_VALUE);
  PowerData_read(SUN_METER, &p);
  powerState.sun_meter_kwh = DIV_METER_CONVERT(p);
  PowerData_read(GRID_METER, &p);
  powerState.grid_meter_kwh = DIV_METER_CONVERT(p);
  
  PowerData_read(R_VOLTAGE, &v);
  powerState.ref_voltage = DIV_REF_CONVERT(v);
  PowerData_read(R_CURRENT, &v);
  powerState.ref_current = DIV_REF_CONVERT(v);
	powerState.power_version = NECTAR_POWER_BOARD_VERSION;
  
  StreamObject _powerState(&powerState, sizeof(powerState));
  printf("-> MAIN %f %f %f %f %f %d %ld %d %d %f %f %f %f %1.2f\r\n",
    powerState.sun_power,
    powerState.sun_voltage,
    powerState.sun_current,
    powerState.pwm_duty,
    powerState.device_temperature,
    (powerState.transistor_overheat_on == COMMS_TRUE_VALUE ? true : false),
    powerState.power_board_error_code,
    (powerState.device_calibrated == COMMS_TRUE_VALUE ? true : false),
    (powerState.pwm_generator_on == COMMS_TRUE_VALUE ? true : false),
    powerState.sun_meter_kwh,
    powerState.grid_meter_kwh,
    powerState.ref_voltage,
    powerState.ref_current,
		powerState.power_version);
  data_setSafeToReadTemp(1);
  return _powerState;
}
