#include "mbed.h"
#include "ServiceComms.h"
#include "DeviceMode.h"
#include "menu_service.h"

static const float GRID_ON_TEMPERATURE = 30.0;
static const float SUN_ON_TEMPERATURE = 60.0;
static const float IDLE_ON_TEMPERATURE = 80.0;

static const uint8_t TURN_ON_GRID = 0x67;
static const uint8_t TURN_ON_SUN = 0x73;
static const uint8_t TURN_ON_IDLE = 0x69;
static const uint8_t ENTER_TEST_MODE = 0x74;

static const PinName PC_TX = PA_2;
static const PinName PC_RX = PA_3;

RawSerial pc(PC_TX, PC_RX);

void parseCommand(uint8_t command) {
  switch(command) {
    case ENTER_TEST_MODE:
      printf("ENTER TEST MODE\r\n");
      if(!deviceOpMode.isInTestStand()) {
        deviceOpMode.setInTestStand();
        service::fakeTemperature = IDLE_ON_TEMPERATURE;
        service::newValueAvailable = true;
        menu_service::enterTestScreen();
      }
      break;
    
    case TURN_ON_GRID:
      printf("TURN ON GRID\r\n");
      if(deviceOpMode.isInTestStand()) {
        service::fakeTemperature = GRID_ON_TEMPERATURE;
        service::newValueAvailable = true;
        menu_service::needUpdate = true;
      }
      break;
    
    case TURN_ON_SUN:
      printf("TURN ON SUN\r\n");
      if(deviceOpMode.isInTestStand()) {
        service::fakeTemperature = SUN_ON_TEMPERATURE;
        service::newValueAvailable = true;
        menu_service::needUpdate = true;
      }
      break;
    
    case TURN_ON_IDLE:
      printf("TURN ON IDLE\r\n");
      if(deviceOpMode.isInTestStand()) {  
        service::fakeTemperature = IDLE_ON_TEMPERATURE;
        service::newValueAvailable = true;
        menu_service::needUpdate = true;
      }
      break;
    
    default:
      printf("Bad command\r\n");
      break;
  }
}

void Rx_interrupt() {
  while(pc.readable()) {
    __disable_irq();
    char rcv = pc.getc();
    printf("%c\r\n", rcv);
    __enable_irq();
    parseCommand(rcv);
  }
}

namespace service {
  float fakeTemperature = IDLE_ON_TEMPERATURE;
  bool newValueAvailable = false;
  
  void setup() {
    static const uint32_t pc_baud = 115200;
    
    pc.baud(pc_baud);
    pc.attach(&Rx_interrupt);
    printf("\r\n[START]\r\n");
    printf("[COMMS PC]\r\n");
    printf("[ok] baud %d\r\n\n", pc_baud);
  }
  
  float getFakeTemperature() {
    return fakeTemperature;
  }
  
  bool isNewValueAvailable() {
    if(newValueAvailable) {
      newValueAvailable = false;
      return true;
    } else return false;
  }
}
