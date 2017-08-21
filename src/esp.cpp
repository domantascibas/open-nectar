#include "mbed.h"
#include "data.h"
#include "esp.h"

static const PinName TX = PA_9;
static const PinName RX = PA_10;

static const uint8_t SET_MODE = 0x4D;       //'M'
static const uint8_t GET_STATS = 0x54;      //'T'

RawSerial esp_serial(TX, RX);

namespace esp {
  static volatile char command = NULL;

  void Rx_interrupt() {
    command = esp_serial.getc();
  }

  //can send 64 chars max
  void send_stats() {
    __disable_irq();
    esp_serial.printf("#%.2f,%d,%.2f,%d,%.2f,%.2f,%.2f,%d,%.2f,%.2f,%.2f$\n",
      data.pv_power, data.grid_relay_on, data.temp_boiler, data.sun_relay_on, data.pv_voltage, data.pv_current,
      data.device_temperature, data.mosfet_overheat_on, data.radiator_temp, data.pwm_duty, data.airgap_temp
    );
    __enable_irq();
    
    printf("V:%5.2f I:%5.2f D:%5.2f Tmosfet:%5.2f Overheat:%d Tairgap:%5.2f\r\n",
    data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp);
  }

  void setup() {
    static const uint16_t baudrate = 19200;
    
    esp_serial.baud(baudrate);
    esp_serial.attach(&Rx_interrupt);
  }

  void loop() {
    if(command != NULL) {
      printf("cmd from esp: 0x%X\r\n", command);
      switch(command) {
        case GET_STATS:
          send_stats();
        break;

        default:
          //do nothing
        break;
      }
      command = NULL;
    }
  }
}

// *******************************Nectar Sun Copyright � Nectar Sun 2017*************************************   
