#include "mbed.h"
#include "data.h"
#include "service.h"
#include "MpptController.h"
#include "device_modes.h"

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

static const PinName PC_TX = PA_2;
static const PinName PC_RX = PA_3;

RawSerial pc(PC_TX, PC_RX);

void parseCommand(uint8_t command) {
  switch(command) {
    case CALIBRATE:
      device_modes::testingStandCalibrate();
      printf("\r\nCALIBRATING POWER BOARD\r\n");
      break;
    
    case INCREASE_DUTY:
      if(data.current_state == MANUAL) {
        mppt.manualIncreaseDuty(false);
        printf("\r\nPWM DUTY INCREASED %f\r\n\n", mppt.get_duty());
      } else printf("[AUTO MODE] press 'm' to enter manual mode\r\n");
      break;
    
    case DECREASE_DUTY:
      if(data.current_state == MANUAL) {
        mppt.manualDecreaseDuty(false);
        printf("\r\nPWM DUTY DECREASED %f\r\n\n", mppt.get_duty());
      } else printf("[AUTO MODE] press 'm' to enter manual mode\r\n");
      break;
      
    case INCREASE_DUTY_FINE:
      if(data.current_state == MANUAL) {
        mppt.manualIncreaseDuty();
        printf("\r\nPWM DUTY INCREASED %f\r\n\n", mppt.get_duty());
      } else printf("[AUTO MODE] press 'm' to enter manual mode\r\n");
      break;
      
    case DECREASE_DUTY_FINE:
      if(data.current_state == MANUAL) {
        mppt.manualDecreaseDuty();
        printf("\r\nPWM DUTY DECREASED %f\r\n\n", mppt.get_duty());
      } else printf("[AUTO MODE] press 'm' to enter manual mode\r\n");
      break;
    
    case RESET_DUTY:
      if(data.current_state == MANUAL) {
        mppt.reset();
        printf("\r\nPWM DUTY RESET %f\r\n\n", mppt.get_duty());
      } else printf("[AUTO MODE] press 'm' to enter manual mode\r\n");
      break;
    
    case MANUAL_MODE:
      data.current_state = MANUAL;
      printf("\r\n[MANUAL MODE] press 'a' to exit manual mode\r\n\n");
      break;
    
    case AUTO_MODE:
      data.current_state = IDLE;
      printf("\r\n[AUTO MODE] press 'm' to enter manual mode\r\n\n");
      break;
    
    case PWM_GENERATOR_START:
      if(data.current_state == MANUAL) {
        mppt.manualStartPwm();
        printf("\r\nPWM GENERATOR STARTED\r\n\n");
      } else printf("[AUTO MODE] press 'm' to enter manual mode\r\n");
      break;
    
    case PWM_GENERATOR_STOP:
      if(data.current_state == MANUAL) {
        mppt.manualStopPwm();
        printf("\r\nPWM GENERATOR STOPPED\r\n\n");
      } else printf("[AUTO MODE] press 'm' to enter manual mode\r\n");
      break;
    
    default:
      if(data.current_state == MANUAL) printf("[MANUAL MODE] press 'a' to exit manual mode\r\n");
      else printf("[AUTO MODE] press 'm' to enter manual mode\r\n");
      break;
  }
}

void Rx_interrupt() {
  if(data.isInOnboarding || data.isTestMode) {
    __disable_irq();
    while(pc.readable()) {
      char rcv = pc.getc();
      parseCommand(rcv);
    }
    __enable_irq();
  } else {
    printf("[AUTO MODE] for manual control put device in TEST MODE or START ONBOARDING\r\n");
  }
}

namespace service {
  void setup() {
    static const uint32_t pc_baud = 115200;
    
    pc.baud(pc_baud);
    pc.attach(&Rx_interrupt);
    printf("\r\n[START]\r\n");
    printf("[COMMS PC]\r\n");
    printf("[ok] baud %d\r\n\n", pc_baud);
  }
}
