#include "mbed.h"
#include "data.h"
#include "comms.h"
#include "power_board.h"
#include "device_modes.h"
#include "hardware.h"
#include "stat_counter.h"

static const PinName TX = PB_10;
static const PinName RX = PB_11;

static const uint8_t POWER_BOARD_STOP = 0x30;       //'0'
static const uint8_t POWER_BOARD_START = 0x31;      //'1'
static const uint8_t GET_DATA = 0x32;               //'2'
static const uint8_t GET_REF_DATA = 0x33;           //'3'
static const uint8_t ENTER_SERVICE_MODE = 0x6D;     //'m'

static const uint8_t INCREASE_PWM = 0x2B;           //'+'
static const uint8_t DECREASE_PWM = 0x2D;           //'-'
static const uint8_t CLEAR_ERROR = 0x20;            //'SPACE'

Ticker get_data_tick;
Timeout error_timeout;
RawSerial power_board_serial(TX, RX);

namespace power_board {
  volatile bool new_data = false;
  volatile bool new_ref_data = false;
  volatile bool error_clearing = false;
  static const uint8_t num_chars = 128;
  static const uint8_t num_fields = 12;
  static uint8_t error_counter = 0;
  char* power_response[num_fields];
  char received_chars[num_chars];
  
  void Rx_interrupt() {
    static const char start_marker = '#';
    static const char end_marker = '$';
    static const char ref_data_marker = '&';
    static bool recv_in_progress = false;
    static uint8_t ndx = 0;
    
    char rc;
    rc = power_board_serial.getc();

    if(recv_in_progress) {
      if(rc != end_marker) {
        if(rc == ref_data_marker) {
          new_ref_data = true;
        } else {
          received_chars[ndx] = rc;
          ndx++;
          printf("%c", rc);
        }
      } else {
        while(power_board_serial.readable()) {
          power_board_serial.getc();
        }
        ndx = 0;
        recv_in_progress = false;
        hardware::available = true;
        new_data = true;
        printf("\r\n");
      }
    } else if(rc == start_marker){
      recv_in_progress = true;
      hardware::available = false;
    }
  }

  void get_data_ISR() {
    comms::send_command(GET_DATA);
  }
  
  void error_timeout_handler() {
    comms::send_command(CLEAR_ERROR);
    error_clearing = false;
  }
  
  void start() {
    comms::send_command(POWER_BOARD_START);
  }
  
  void stop() {
    comms::send_command(POWER_BOARD_STOP);
  }

  void setup() {
    static const uint16_t baudrate = 19200;
    static const float interval = 1.0;
    
    power_board_serial.baud(baudrate);
    comms::send_command(POWER_BOARD_START);
    get_data_tick.attach(&get_data_ISR, interval);
    power_board_serial.attach(&Rx_interrupt);
  }

  void loop() {
    uint8_t message_count;
    
    if((data.error != 0x00) & (!error_clearing)) {
      if(error_counter < 5) {
        error_clearing = true;
        comms::print_error(data.error);
        printf("ERROR COUNTER ++\r\n");
        error_timeout.attach(&error_timeout_handler, 10.0);
        error_counter++;
      } else {
        comms::send_command(POWER_BOARD_STOP);
        data.error = RESTART_REQUIRED;
      }
    } else if((data.error == 0x00) & (error_counter != 0)) {
      error_counter = 0;
      printf("ERROR COUNTER RESET\r\n");
    }

    if(new_data) {
      new_data = false;
      if(new_ref_data) {
        new_ref_data = false;
        printf(" [PARSING REF] ");
        message_count = comms::parse_fields(received_chars, power_response, num_fields, ',');
        if(message_count == 2) {
          __disable_irq();
          data.pv_ref_voltage = atof(power_response[0]);
          data.pv_ref_current = atof(power_response[1]);
          __enable_irq();
          printf("parsed\r\n");
        
        } else {
          printf("[ERROR] partial message received\r\n");
        }
      } else {
        printf(" [PARSING] ");
        message_count = comms::parse_fields(received_chars, power_response, num_fields, ',');
        if(message_count == 9) {
          __disable_irq();
          data.pv_voltage = atof(power_response[0]);
          data.pv_current = atof(power_response[1]);
          data.pwm_duty = atof(power_response[2]);
          data.radiator_temp = atof(power_response[3]);
          data.mosfet_overheat_on = atoi(power_response[4]);
          data.airgap_temp = atof(power_response[5]);
          data.error = atoi(power_response[6]);
          data.calibrated = atoi(power_response[7]);
          data.generator_on = atoi(power_response[8]);
          
          data.pv_power = data.pv_voltage * data.pv_current;
          __enable_irq();
          printf("parsed\r\n");
          stat_counter::increase();
        } else {
          printf("[ERROR] partial message received\r\n");
        }
      }
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
