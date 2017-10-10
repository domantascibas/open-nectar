#include "mbed.h"
#include "data.h"
#include "power_board.h"

#include "NectarStream.h"
#include "NectarContract.h"

Ticker get_data_tick;
Timeout line_busy_timeout;
Timeout error_timeout;

void line_busy_ISR() {
  line_busy = false;
}

namespace power_board {
  static const PinName TX = PB_10;
  static const PinName RX = PB_11;
  
  volatile bool error_clearing = false;
  static uint8_t error_counter = 0;
  
  powerStream m_stream(TX, RX);
  
  void error_timeout_handler() {
    m_stream.stream.sendObject(C_SERVICE_CLEAR_ERROR);
    error_clearing = false;
  }
  
  void start() {
    m_stream.stream.sendObject(C_POWER_BOARD_START);
  }
  
  void stop() {
    m_stream.stream.sendObject(C_POWER_BOARD_STOP);
  }
  
  void send_grid_meter() {
    nectar_contract::PowerBoardGridMeter grid_meter = {
      data.grid_kwh
    };
    
    m_stream.stream.sendObject(C_POWER_BOARD_GRID_METER, &grid_meter, sizeof(grid_meter));
  }
  
  void get_data_ISR() {
    m_stream.stream.sendObject(C_POWER_BOARD_STATS);
    line_busy = true;
    line_busy_timeout.attach(line_busy_ISR, 0.05);
  }
  
  void config() {
  }
  
  void print_error(uint8_t error) {
    printf("[ERROR] %d\r\n", error);
    switch(error) {
      case SETUP_ERROR:
        printf("[ERROR] SETUP ERROR\r\n");
      break;

      case STARTUP_ERROR:
        printf("[ERROR] STARTUP ERROR\r\n");
      break;

      case ADC_ERROR:        //can't find both ADC sensors
        printf("[ERROR] Can't read ADC sensors\r\n");
      break;

      case ADC_VOLTAGE_ERROR:
        printf("[ERROR] Can't read ADC voltage sensor\r\n");
      break;

      case ADC_CURRENT_ERROR:
        printf("[ERROR] Can't read ADC voltage sensor\r\n");
      break;

      case ADC_SETUP_ERROR:
        printf("[ERROR] ADC SETUP ERROR\r\n");
      break;

      case FLASH_ACCESS_ERROR:
        printf("[ERROR] Can't access FLASH memory\r\n");
      break;

      case FLASH_READ_ERROR:
        printf("[ERROR] Can't read FLASH memory\r\n");
      break;

      case FLASH_WRITE_ERROR:
        printf("[ERROR] Can't write to FLASH memory\r\n");
      break;

      case CALIBRATION_ERROR:          //no calibration data
        printf("[ERROR] No CALIBRATION data\r\n");
      break;

      case DC_OVER_VOLTAGE:            //V_pv > 350V
        printf("[ERROR] DC VOLTAGE > 350V\r\n");
      break;

      case DC_OVER_CURRENT:            //I_pv > 10A
        printf("[ERROR] DC CURRENT > 10A\r\n");
      break;

      case DC_CURRENT_LEAKS:           //could be a faulty relay, or a short
        printf("[ERROR] DC CURRENT leaks. Could be faulty relay or a short circuit\r\n");
      break;

      case I2C_ERROR:
        printf("[ERROR] I2C ERROR\r\n");
      break;

      case OVERHEAT:
        printf("[ERROR] DEVICE OVERHEAT\r\n");
      break;

      case RADIATOR_OVERHEAT:
        printf("[ERROR] Radiator TEMPERATURE > 70C\r\n");
      break;

      case AIRGAP_OVERHEAT:
        printf("[ERROR] Airgap TEMPERATURE > 70C\r\n");
      break;
      
      case NO_LOAD:
        printf("[ERROR] Load disconnected\r\n");
      break;

      default:
        printf("[ERROR] OTHER ERROR\r\n");
      break;
    }
  }
  
  void setup() {
    m_stream.setup();
  }
  
  void loop() {
    if((data.error != 0x00) & (!error_clearing)) {
      if(error_counter < 5) {
        error_clearing = true;
        print_error(data.error);
        printf("ERROR COUNTER ++\r\n");
        error_timeout.attach(&error_timeout_handler, 10.0);
        error_counter++;
      } else {
        m_stream.stream.sendObject(C_POWER_BOARD_STOP);
        data.error = RESTART_REQUIRED;
      }
    } else if((data.error == 0x00) & (error_counter != 0)) {
      error_counter = 0;
      printf("ERROR COUNTER RESET\r\n");
    } 
  }
}

/*
 * Class implementation
 */

void powerStream::setup() {
  static const float interval = 1.0;
  
  m_serial.baud(C_SERIAL_BAUD_RATE);
  stream.sendObject(C_POWER_BOARD_STATS);
  line_busy = true;
  line_busy_timeout.attach(line_busy_ISR, 0.05);
  get_data_tick.attach(&power_board::get_data_ISR, interval);
  
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

void powerStream::received_power_stats(const nectar_contract::PowerBoardStats &stats) {
  //power_data::stats = stats;

  __disable_irq();
  data.pv_power = stats.sun_power;
  data.pv_voltage = stats.sun_voltage;
  data.pv_current = stats.sun_current;
  data.pwm_duty = stats.pwm_duty;
  data.radiator_temp = stats.radiator_temperature;
  data.mosfet_overheat_on = stats.transistor_overheat_on;
  data.error = stats.power_board_error_code;
  data.calibrated = stats.device_calibrated;
  data.generator_on = stats.pwm_generator_on;
  data.solar_kwh = stats.sun_meter_kwh;
  __enable_irq();
  
  printf("%.3f %.2f %.2f %.3fkWh %d %d %d\r\n", data.pv_voltage, data.pv_current, data.pwm_duty, data.solar_kwh, data.error, data.generator_on, data.calibrated);
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   

/*
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
//        hardware::available = true;
        new_data = true;
        printf("\r\n");
      }
    } else if(rc == start_marker){
      recv_in_progress = true;
//      hardware::available = false;
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
    comms::send_command(GET_DATA);
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
    
  }
}

*/

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
