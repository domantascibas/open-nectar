#include "PowerBoardComms.h"
#include "error_controller.h"
#include "DataService.h"
#include "EnergyMeter.h"
#include "CommsController.h"

namespace power_board {
  // Ticker get_data_tick;
  // bool received_first_msg = false;
  // static bool startPowerBoard = false;
  
  // powerStream m_stream(POWER_COMMS_TX_PIN, POWER_COMMS_RX_PIN);
  
  void get_data_ISR() {
    // commsController.sendPowerMessage();
  }
  
  void start() {
    // startPowerBoard = true;
    // // printf("[POWER BOARD] start\r\n");
  }
  
  void stop() {
    // startPowerBoard = false;
  }
  
  void setup() {
    // m_stream.setup();
  }
  
  bool receivedFirstMessage() {
    // return received_first_msg;
  }
  
  void send_message() {
    // nectar_contract::MainBoardStateForPower mainStateForPower = {
    //   gridMeter.getMeterReading(),
    //   (startPowerBoard ? COMMS_TRUE_VALUE : COMMS_FALSE_VALUE),
    //   (deviceOpMode.isInTestMode() ? COMMS_TRUE_VALUE : COMMS_FALSE_VALUE),
    //   (deviceOpMode.isOnboarding() ? COMMS_TRUE_VALUE : COMMS_FALSE_VALUE),
		// 	(DataService::getCalibrate() ? COMMS_TRUE_VALUE : COMMS_FALSE_VALUE)
    // };
    
    // StreamObject _mainStateForPower(&mainStateForPower, sizeof(mainStateForPower));
    // m_stream.stream.send_state_to_power_board(_mainStateForPower);
    // printf("POWER COMMAND %f %d %d %d %d\r\n",
    //   mainStateForPower.grid_meter_kwh,
    //   (mainStateForPower.start == COMMS_TRUE_VALUE ? true : false),
    //   (mainStateForPower.is_test_mode_on == COMMS_TRUE_VALUE ? true : false),
    //   (mainStateForPower.is_in_onboarding == COMMS_TRUE_VALUE ? true : false),
    //   (mainStateForPower.calibrate == COMMS_TRUE_VALUE ? true : false)
    // );
		
		// if(DataService::getCalibrate()) {
		// 	DataService::setCalibrate(false);
		// }
  }
  
  bool is_message_valid(const nectar_contract::PowerBoardState &state) {
    // if((state.grid_meter_kwh > 100000000) ||
    //     (state.grid_meter_kwh < 0) ||
    //     (state.device_temperature > 1000) ||
    //     (state.device_temperature < -1) ||
    //     (state.pwm_duty > 1.0) ||
    //     (state.pwm_duty < 0) ||
    //     (state.ref_current > 1000) ||
    //     (state.ref_current < 0) ||
    //     (state.ref_voltage > 1000) ||
    //     (state.ref_voltage < 0) ||
    //     (state.sun_current > 1000) ||
    //     (state.sun_current < 0) ||
    //     (state.sun_meter_kwh > 100000000) ||
    //     (state.sun_meter_kwh < 0) ||
    //     (state.sun_power > 100000) ||
    //     (state.sun_power < 0) ||
    //     (state.sun_voltage > 100000) ||
    //     (state.sun_voltage < 0) ||
    //     (state.power_board_error_code > MAX_ERROR_CODE)
    // )
    // {
    //   return false;
    // } else {
    //   return true;      
    // }
  }
}

/*
 * Class implementation
 */

void powerStream::setup() {
  // m_serial.baud(C_SERIAL_BAUD_RATE);
  // power_board::get_data_tick.attach(&power_board::get_data_ISR, POWER_COMMS_PING_INTERVAL);
  
  // m_serial.attach(callback(this, &powerStream::Rx_interrupt));
  // printf("POWER START\n");
}

void powerStream::Rx_interrupt() {
//   while(m_serial.readable()) {
//     __disable_irq();
//     char rcv = m_serial.getc();
// //    printf("%c", rcv);
//     stream.receiveChar(rcv);
//     __enable_irq();
//   }
}

void powerStream::write(uint8_t byte) {
// //  printf("%c", byte);
//   m_serial.putc(byte);
// }

// void powerStream::received_power_board_state(const nectar_contract::PowerBoardState &state) {
//   if(power_board::is_message_valid(state)) {
//     __disable_irq();
// //    powerData = state;
// 		powerData.sun_power = state.sun_power;
// 		powerData.sun_voltage = state.sun_voltage;
// 		powerData.sun_current = state.sun_current;
// 		powerData.pwm_duty = state.pwm_duty;
// 		powerData.device_temperature = state.device_temperature;
// 		powerData.transistor_overheat_on = (state.transistor_overheat_on == COMMS_TRUE_VALUE ? true : false);
// 		powerData.device_calibrated = (state.device_calibrated == COMMS_TRUE_VALUE ? true : false);
// 		powerData.pwm_generator_on = (state.pwm_generator_on == COMMS_TRUE_VALUE ? true : false);
// 		powerData.sun_meter_kwh = state.sun_meter_kwh;
// 		powerData.grid_meter_kwh = state.grid_meter_kwh;
// 		powerData.ref_voltage = state.ref_voltage;
// 		powerData.ref_current = state.ref_current;
// 		powerData.power_version = state.power_version;
		
// 		powerData.power_board_error_code = state.power_board_error_code;
//     powerBoardError.save_error_code(state.power_board_error_code);
    
//     if(!power_board::received_first_msg || !gridMeter.isMeterSet() || (powerData.grid_meter_kwh > gridMeter.getMeterReading())) gridMeter.setMeterReading(powerData.grid_meter_kwh);
//     DataService::calculateSolarKwhDiff(!power_board::received_first_msg);
//     __enable_irq();
//     printf("POWER RESPONSE %f %f %f %f %f %f %f %f %f %1.2f %d %d %d %d\r\n",
//       powerData.sun_power,
//       powerData.sun_voltage,
//       powerData.sun_current,
//       powerData.pwm_duty,
//       powerData.device_temperature,
//       powerData.sun_meter_kwh,
//       powerData.grid_meter_kwh,
//       powerData.ref_voltage,
//       powerData.ref_current,
// 			powerData.power_version/100,
//       powerBoardError.get_errors(),
//       powerData.device_calibrated,
//       powerData.pwm_generator_on,
//       powerData.transistor_overheat_on
//     );
//     if(!power_board::received_first_msg) power_board::received_first_msg = true;
//   // } else {
//   //   printf("POWER_CMD RECEIVED INVALID DATA IN MESSAGE\r\n");
//   }
//   commsController.freeChannel();
}