#include "EspComms.h"
#include "device_modes.h"
#include "error_controller.h"
// #include "menu_service.h"
#include "DataService.h"
#include "CommsController.h"
#include "consts.h"

// extern RelayController relayController;

namespace esp {
  // Ticker get_data_tick;
  // bool received_first_msg = false;
  
  // mbedStream m_stream(ESP_COMMS_TX_PIN, ESP_COMMS_RX_PIN);
  
  void get_data_ISR() {
    // commsController.sendEspMessage();
  }

  void setup() {
    // m_stream.setup();
  }
  
  void send_message() {
    // time_t rtc = time(NULL);
    // nectar_contract::MainBoardStateForESP mainStateForEsp = {
    //   powerData.sun_power,
    //   (relayController.isGridRelayOn() ? COMMS_TRUE_VALUE : COMMS_FALSE_VALUE),
    //   temperatureData.getBoilerTemperature(),
    //   (relayController.isSunRelayOn() ? COMMS_TRUE_VALUE : COMMS_FALSE_VALUE),
    //   powerData.sun_voltage,
    //   powerData.sun_current,
    //   temperatureData.getDeviceTemperature(),
    //   (powerData.transistor_overheat_on ? COMMS_TRUE_VALUE : COMMS_FALSE_VALUE),
    //   powerData.pwm_duty,
    //   powerBoardError.get_errors(),
    //   mainBoardError.get_errors(),
    //   powerData.sun_meter_kwh,
    //   powerData.grid_meter_kwh,
    //   rtc,
    //   (deviceOpMode.isReset() ? COMMS_TRUE_VALUE : COMMS_FALSE_VALUE),
    //   (deviceOpMode.isPairing() ? COMMS_TRUE_VALUE : COMMS_FALSE_VALUE),
    //   (deviceOpMode.isBoostOff() ? COMMS_TRUE_VALUE : COMMS_FALSE_VALUE),
		// 	powerData.ref_voltage,
		// 	powerData.ref_current,
		// 	(uint16_t)NECTAR_MAIN_BOARD_VERSION
    // };
    
    // StreamObject _mainStateForEsp(&mainStateForEsp, sizeof(mainStateForEsp));
    // m_stream.stream.send_state_to_esp(_mainStateForEsp);
    // // printf("\r\nCurrent time: %s\r", ctime(&rtc));
    // printf("ESP COMMAND %d %f %f %f %f %f %f %f %f %f %f %d %d %d %d %d %d %d %d %d\n",
    //   mainStateForEsp.time,
    //   mainStateForEsp.moment_sun_watt,
    //   mainStateForEsp.sun_voltage,
    //   mainStateForEsp.sun_current,
    //   mainStateForEsp.pwm_duty,
    //   mainStateForEsp.water_temperature,
    //   mainStateForEsp.device_temperature,
    //   mainStateForEsp.sun_meter_kwh,
    //   mainStateForEsp.grid_meter_kwh,
    //   mainStateForEsp.ref_current,
    //   mainStateForEsp.ref_current,
    //   mainStateForEsp.main_version,
    //   mainStateForEsp.main_board_error_code,
    //   mainStateForEsp.power_board_error_code,
		// 	(mainStateForEsp.sun_relay_on == COMMS_TRUE_VALUE ? true : false),
    //   (mainStateForEsp.grid_relay_on == COMMS_TRUE_VALUE ? true : false),
    //   (mainStateForEsp.pair_mode == COMMS_TRUE_VALUE ? true : false),
    //   (mainStateForEsp.send_boost_off == COMMS_TRUE_VALUE ? true : false),
    //   (mainStateForEsp.reset == COMMS_TRUE_VALUE ? true : false),
    //   (mainStateForEsp.transistor_overheat_on == COMMS_TRUE_VALUE ? true : false)
    //   );
    // if(deviceOpMode.isReset()) deviceOpMode.setReset(false);
  }
  
  bool is_message_valid(const nectar_contract::ESPState &state) {
    // if((state.boiler_power > 3000) ||
    //     (state.boiler_power < 0) ||
    //     (state.pin >= 10000) ||
    //     (state.pin < 1000) ||
    //     (state.temperature > 1000) ||
    //     (state.temperature < 0) ||
    //     (state.temperature_max > 100) ||
    //     (state.temperature_max <= 10)
    // )
    // {
    //   return false;
    // } else {
    //   return true;
    // }
  }

  bool receivedFirstMessage() {
    // return received_first_msg;
  }
}

/*
 * Class implementation
 */

void mbedStream::setup() {
  // m_serial.baud(C_SERIAL_BAUD_RATE);
  // esp::get_data_tick.attach(&esp::get_data_ISR, ESP_COMMS_PING_INTERVAL);
  // m_serial.attach(callback(this, &mbedStream::Rx_interrupt));
  // printf("ESP START\n");
  // esp::send_message();
}

void mbedStream::Rx_interrupt() {
//   while(m_serial.readable()) {
//     __disable_irq();
//     char rcv = m_serial.getc();
// //    printf("%c", rcv);
//     stream.receiveChar(rcv);
//     __enable_irq();
//   }
}

void mbedStream::write(uint8_t byte) {
  m_serial.putc(byte);
}

void mbedStream::received_esp_state(const nectar_contract::ESPState &state) {
  // if(esp::is_message_valid(state)) {
  //   __disable_irq();
  //   espData.pin = state.pin;
	// 	espData.esp_version = state.esp_version;
  //   if(state.is_configured == COMMS_TRUE_VALUE) {
	// 		espData.heater_mode = state.heater_mode;
	// 		espData.is_configured = (state.is_configured == COMMS_TRUE_VALUE ? true : false);
	// 		espData.has_internet_connection = (state.has_internet_connection == COMMS_TRUE_VALUE ? true : false);
	// 		espData.temperature = state.temperature;
	// 		espData.temperature_max = state.temperature_max;
	// 		espData.boiler_power = state.boiler_power;
	// 		espData.sync_time = state.sync_time;
	// 		espData.pin = state.pin;
	// 		float esp_version;
  //     if((nectar_contract::HeaterMode)espData.heater_mode != nectar_contract::Boost)
  //       deviceOpMode.setBoostOff(false);
  //     if(DataService::getCurrentHeaterMode() != (nectar_contract::HeaterMode)espData.heater_mode) {
  //       // printf("HEATER MODE UPDATED\r\n");
  //       DataService::setCurrentHeaterMode((nectar_contract::HeaterMode)espData.heater_mode);
  //     }
  //     device_modes::updateHeaterMode = true;
  //     printf("ESP RESPONSE %lld %f %f %f %1.2f %d %d %d %d\n",
  //       espData.sync_time,
  //       espData.boiler_power,
  //       espData.temperature,
  //       espData.temperature_max,
  //       espData.esp_version/100,
  //       espData.pin,
  //       espData.heater_mode,
  //       espData.is_configured,
  //       espData.has_internet_connection
  //     );
  //     if(espData.sync_time != 0) {
  //       set_time(espData.sync_time);
  //       time_t sec = time(NULL);
  //       menu_actions::updateTime();
  //       // printf("Time set from ESP: %s\r\n", ctime(&sec));
  //     }
  //     __enable_irq();
  //   } else {
  //     printf("ESP RESPONSE %lld %f %f %f %1.2f %d %d %d %d\n",
  //       espData.sync_time,
  //       espData.boiler_power,
  //       espData.temperature,
  //       espData.temperature_max,
  //       espData.esp_version/100,
  //       espData.pin,
  //       espData.heater_mode,
  //       espData.is_configured,
  //       espData.has_internet_connection
  //     );
  //   }
  //   if(!esp::received_first_msg) esp::received_first_msg = true;
  // // } else {
  // //   printf("ESP -> RECEIVED INVALID DATA IN MESSAGE\r\n");
  // }
  // commsController.freeChannel();
}
