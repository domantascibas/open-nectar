#include "consts.h"
#include "ServiceComms.h"
#include "DeviceMode.h"
#include "menu_service.h"

RawSerial pc(SERVICE_COMMS_TX_PIN, SERVICE_COMMS_RX_PIN);

void parseCommand(uint8_t command) {
  switch(command) {
    case TEST_MODE_ENTER_TEST_MODE_CMD:
      printf("ENTER TEST MODE\r\n");
      if(!deviceOpMode.isInTestStand()) {
        deviceOpMode.setInTestStand();
        service::fakeTemperature = TEST_MODE_IDLE_ON_TEMPERATURE;
        service::newValueAvailable = true;
		menu_actions::modeSelected();
        menu_service::enterTestScreen();
      }
      break;
    
    case TEST_MODE_TURN_ON_GRID_CMD:
      printf("TURN ON GRID\r\n");
      if(deviceOpMode.isInTestStand()) {
        service::fakeTemperature = TEST_MODE_GRID_ON_TEMPERATURE;
        service::newValueAvailable = true;
        menu_service::needUpdate = true;
      }
      break;
    
    case TEST_MODE_TURN_ON_SUN_CMD:
      printf("TURN ON SUN\r\n");
      if(deviceOpMode.isInTestStand()) {
        service::fakeTemperature = TEST_MODE_SUN_ON_TEMPERATURE;
        service::newValueAvailable = true;
        menu_service::needUpdate = true;
      }
      break;
    
    case TEST_MODE_TURN_ON_IDLE_CMD:
      printf("TURN ON IDLE\r\n");
      if(deviceOpMode.isInTestStand()) {  
        service::fakeTemperature = TEST_MODE_IDLE_ON_TEMPERATURE;
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
  float fakeTemperature = TEST_MODE_IDLE_ON_TEMPERATURE;
  bool newValueAvailable = false;
  
  void setup() {
    static const uint32_t pc_baud = SERVICE_COMMS_BAUD_RATE;
    
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
