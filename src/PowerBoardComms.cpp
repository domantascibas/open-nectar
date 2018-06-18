#include "PowerBoardComms.h"
#include "ErrorHandler.h"
#include "DataService.h"
#include "EnergyMeter.h"
#include "CommsController.h"

namespace power_board {
  Ticker get_data_tick;
  bool received_first_msg = false;

  static bool startPowerBoard = false;
  static const PinName TX = PB_10;
  static const PinName RX = PB_11;
  
  powerStream m_stream(TX, RX);
  
  void get_data_ISR() {
    commsController.sendPowerMessage();
  }
  
  void start() {
    startPowerBoard = true;
    printf("[POWER BOARD] start\r\n");
  }
  
  void stop() {
    startPowerBoard = false;
  }
  
  void setup() {
    m_stream.setup();
  }
  
  bool receivedFirstMessage() {
    return received_first_msg;
  }
  
  void send_message() {
    nectar_contract::MainBoardStateForPower mainStateForPower = {
      gridMeter.getMeterReading(),
      (startPowerBoard ? 0xAC : 0xFA),
      (deviceOpMode.isInTestMode() ? 0xAC : 0xFA),
      (deviceOpMode.isOnboarding() ? 0xAC : 0xFA),
			(DataService::getCalibrate() ? 0xAC : 0xFA)
    };
    
    StreamObject _mainStateForPower(&mainStateForPower, sizeof(mainStateForPower));
    m_stream.stream.send_state_to_power_board(_mainStateForPower);
    printf("-> POWER %f %d %d %d %d\r\n",
    mainStateForPower.grid_meter_kwh,
    (mainStateForPower.start == 0xAC ? true : false),
    (mainStateForPower.is_test_mode_on == 0xAC ? true : false),
    (mainStateForPower.is_in_onboarding == 0xAC ? true : false),
		(mainStateForPower.calibrate == 0xAC ? true : false));
		
		if(DataService::getCalibrate()) {
			DataService::setCalibrate(false);
		}
  }
  
  bool is_message_valid(const nectar_contract::PowerBoardState &state) {
    if((state.grid_meter_kwh > 100000000) ||
        (state.grid_meter_kwh < 0) ||
        (state.device_temperature > 1000) ||
        (state.device_temperature < -1) ||
        (state.pwm_duty > 1.0) ||
        (state.pwm_duty < 0) ||
        (state.ref_current > 1000) ||
        (state.ref_current < 0) ||
        (state.ref_voltage > 1000) ||
        (state.ref_voltage < 0) ||
        (state.sun_current > 1000) ||
        (state.sun_current < 0) ||
        (state.sun_meter_kwh > 100000000) ||
        (state.sun_meter_kwh < 0) ||
        (state.sun_power > 100000) ||
        (state.sun_power < 0) ||
        (state.sun_voltage > 100000) ||
        (state.sun_voltage < 0) ||
        (state.power_board_error_code > 8191)
    )
    {
      return false;
    } else {
      return true;      
    }
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
  if(power_board::is_message_valid(state)) {
    __disable_irq();
//    powerData = state;
		powerData.sun_power = state.sun_power;
		powerData.sun_voltage = state.sun_voltage;
		powerData.sun_current = state.sun_current;
		powerData.pwm_duty = state.pwm_duty;
		powerData.device_temperature = state.device_temperature;
		powerData.transistor_overheat_on = (state.transistor_overheat_on == 0xAC ? true : false);
		powerData.power_board_error_code = state.power_board_error_code;
		powerData.device_calibrated = (state.device_calibrated == 0xAC ? true : false);
		powerData.pwm_generator_on = (state.pwm_generator_on == 0xAC ? true : false);
		powerData.sun_meter_kwh = state.sun_meter_kwh;
		powerData.grid_meter_kwh = state.grid_meter_kwh;
		powerData.ref_voltage = state.ref_voltage;
		powerData.ref_current = state.ref_current;
		powerData.power_version = state.power_version;
		
    powerBoardError.save_error_code(state.power_board_error_code);
    
    if(!power_board::received_first_msg) gridMeter.setMeterReading(powerData.grid_meter_kwh);
    DataService::calculateSolarKwhDiff(!power_board::received_first_msg);
    __enable_irq();
    printf("POWER -> %f %f %f %f %f %d %d %d %d %f %f %f %f %1.2f\r\n",
      powerData.sun_power,
      powerData.sun_voltage,
      powerData.sun_current,
      powerData.pwm_duty,
      powerData.device_temperature,
      powerData.transistor_overheat_on,
      powerBoardError.get_errors(),
      powerData.device_calibrated,
      powerData.pwm_generator_on,
      powerData.sun_meter_kwh,
      powerData.grid_meter_kwh,
      powerData.ref_voltage,
      powerData.ref_current,
			powerData.power_version/100
    );
    if(!power_board::received_first_msg) power_board::received_first_msg = true;
  } else {
    printf("POWER -> RECEIVDED INVALID DATA IN MESSAGE\r\n");
  }
  commsController.freeChannel();
}
