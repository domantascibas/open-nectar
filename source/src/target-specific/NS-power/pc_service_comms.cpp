#include "consts.h"
// #include "data.h"
#include "pc_service_comms.h"
#include "power_controller.h"
#include "device_modes.h"

extern "C" {
    #include "data.h"
}

void parse_command(uint8_t command);
void Rx_interrupt(void);

static const uint8_t INCREASE_DUTY = 0x2A;
static const uint8_t DECREASE_DUTY = 0x2F;
static const uint8_t INCREASE_DUTY_FINE = 0x2B;
static const uint8_t DECREASE_DUTY_FINE = 0x2D;
static const uint8_t RESET_DUTY = 0x72;

static const uint8_t PWM_GENERATOR_START = 0x31;
static const uint8_t PWM_GENERATOR_STOP = 0x30;

static const uint8_t MANUAL_MODE = 0x6D;
static const uint8_t AUTO_MODE = 0x61;
static const uint8_t CALIBRATE = 0x63;

RawSerial pc(PC_TX, PC_RX);

void pc_service_comms_init(void) {
  pc.baud(PC_BAUD);
  pc.attach(&Rx_interrupt);
  pc.printf("\r\n[START]\r\n");
  pc.printf("[COMMS PC]\r\n");
  pc.printf("[ok] baud %d\r\n\n", PC_BAUD);
}

void parse_command(uint8_t command) {
  // switch(command) {
  //   case CALIBRATE:
  //     data.isCalibrating = true;
  //     data.startCalibration = true;
  //     printf("\r\nCALIBRATING POWER BOARD\r\n");
  //     break;
    
  //   case INCREASE_DUTY:
  //     if(data.current_state == MANUAL) {
  //       power_controller_manual_increase_duty(false);
  //       printf("\r\nPWM DUTY INCREASED %f\r\n\n", power_controller_get_duty());
  //     } else printf("[AUTO MODE] press 'm' to enter manual mode\r\n");
  //     break;
    
  //   case DECREASE_DUTY:
  //     if(data.current_state == MANUAL) {
  //       power_controller_manual_decrease_duty(false);
  //       printf("\r\nPWM DUTY DECREASED %f\r\n\n", power_controller_get_duty());
  //     } else printf("[AUTO MODE] press 'm' to enter manual mode\r\n");
  //     break;
      
  //   case INCREASE_DUTY_FINE:
  //     if(data.current_state == MANUAL) {
  //       power_controller_manual_increase_duty();
  //       printf("\r\nPWM DUTY INCREASED %f\r\n\n", power_controller_get_duty());
  //     } else printf("[AUTO MODE] press 'm' to enter manual mode\r\n");
  //     break;
      
  //   case DECREASE_DUTY_FINE:
  //     if(data.current_state == MANUAL) {
  //       power_controller_manual_decrease_duty();
  //       printf("\r\nPWM DUTY DECREASED %f\r\n\n", power_controller_get_duty());
  //     } else printf("[AUTO MODE] press 'm' to enter manual mode\r\n");
  //     break;
    
  //   case RESET_DUTY:
  //     if(data.current_state == MANUAL) {
  //       power_controller_mppt_stop();
  //       printf("\r\nPWM DUTY RESET %f\r\n\n", power_controller_get_duty());
  //     } else printf("[AUTO MODE] press 'm' to enter manual mode\r\n");
  //     break;
    
  //   case MANUAL_MODE:
  //     data.current_state = MANUAL;
  //     printf("\r\n[MANUAL MODE] press 'a' to exit manual mode\r\n\n");
  //     break;
    
  //   case AUTO_MODE:
  //     data.current_state = IDLE;
  //     printf("\r\n[AUTO MODE] press 'm' to enter manual mode\r\n\n");
  //     break;
    
  //   case PWM_GENERATOR_START:
  //     if(data.current_state == MANUAL) {
  //       power_controller_manual_output_on();
  //       printf("\r\nPWM GENERATOR STARTED\r\n\n");
  //     } else printf("[AUTO MODE] press 'm' to enter manual mode\r\n");
  //     break;
    
  //   case PWM_GENERATOR_STOP:
  //     if(data.current_state == MANUAL) {
  //       power_controller_manual_output_off();
  //       printf("\r\nPWM GENERATOR STOPPED\r\n\n");
  //     } else printf("[AUTO MODE] press 'm' to enter manual mode\r\n");
  //     break;
    
  //   default:
  //     if(data.current_state == MANUAL) printf("[MANUAL MODE] press 'a' to exit manual mode\r\n");
  //     else printf("[AUTO MODE] press 'm' to enter manual mode\r\n");
  //     break;
  // }
}

void Rx_interrupt(void) {
  if(data_getIsInOnboarding() || data_getIsTestMode()) {
    __disable_irq();
    while(pc.readable()) {
      char rcv = pc.getc();
      parse_command(rcv);
    }
    __enable_irq();
//  } else {
//    printf("[AUTO MODE] for manual control put device in TEST MODE or START ONBOARDING\r\n");
  }
}
