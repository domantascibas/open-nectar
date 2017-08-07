#include "mbed.h"
#include "data.h"
#include "power_board.h"
#include "comms.h"

Ticker get_data_tick;

static const PinName TX = PB_10;
static const PinName RX = PB_11;
RawSerial power_board_serial(TX, RX);

static const uint8_t CMD_POWER_BOARD_START = 0x0F;
static const uint8_t CMD_POWER_BOARD_STOP = 0xF0;
static const uint8_t CMD_GET_REF_DATA = 0x43;       //'C'
static const uint8_t CMD_GET_DATA = 0x32;           //'2'   0x44;           //'D'
static const uint8_t CMD_PWM_ON = 0x41;             //'A'
static const uint8_t CMD_PWM_OFF = 0x42;            //'B'

namespace power_board {
  volatile bool new_data = false;
  static const uint8_t num_chars = 128;
  static const uint8_t num_fields = 12;
  char* power_response[num_fields];
  char received_chars[num_chars];
  
  void Rx_interrupt() {
    static const char start_marker = '#';
    static const char end_marker = '$';
    static bool recv_in_progress = false;
    static uint8_t ndx = 0;
    
    char rc;
    rc = power_board_serial.getc();

    if(recv_in_progress) {
      if(rc != end_marker) {
        received_chars[ndx] = rc;
        ndx++;
        printf("%c", rc);
      } else {
        while(power_board_serial.readable()) {
          power_board_serial.getc();
        }
        ndx = 0;
        recv_in_progress = false;
        new_data = true;
      }
    } else if(rc == start_marker){
      recv_in_progress = true;
    }
  }
  
  uint8_t send_command(uint8_t command) {
    //TODO add timeout after sending command
    uint8_t response;
    power_board_serial.putc(command);
    response = power_board_serial.getc();
    if(response == NACK) {
      response = power_board_serial.getc();
      comms::print_error(response);
    }
    return response;
  }

  void stop() {
    send_command(CMD_POWER_BOARD_STOP);
  }

  void get_data_ISR() {
    send_command(CMD_GET_DATA);
  }

  void setup() {
    power_board_serial.baud(19200);
    send_command(CMD_POWER_BOARD_START);
    get_data_tick.attach(&get_data_ISR, 1.0);
    power_board_serial.attach(&Rx_interrupt);
  }

  void loop() {
    uint8_t message_count;

    if(new_data) {
      new_data = false;
      printf(" [PARSING] ");
      message_count = comms::parse_fields(received_chars, power_response, num_fields, ',');
      if(message_count == 6) {
        __disable_irq();
        data.pv_voltage = atof(power_response[0]);
        data.pv_current = atof(power_response[1]);
        data.pwm_duty = atof(power_response[2]);
        data.radiator_temp = atof(power_response[3]);
        data.mosfet_overheat_on = atoi(power_response[4]);
        data.airgap_temp = atof(power_response[5]);

        data.pv_power = data.pv_voltage * data.pv_current;
        __enable_irq();
        printf("parsed\r\n");
      } else {
        printf("[ERROR] partial message received\r\n");
      }
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
