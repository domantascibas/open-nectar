#include "consts.h"
#include "ServiceComms.h"
#include "DeviceMode.h"
// #include "menu_service.h"

// RawSerial pc(SERVICE_COMMS_TX_PIN, SERVICE_COMMS_RX_PIN);

void parseCommand(uint8_t command) {
  // switch(command) {
  //   case TEST_MODE_ENTER_TEST_MODE_CMD:
  //     printf("MODE TEST\n");
  //     if(!deviceOpMode.isInTestStand()) {
  //       deviceOpMode.setInTestStand();
  //       service::fakeTemperature = TEST_MODE_IDLE_ON_TEMPERATURE;
  //       service::newValueAvailable = true;
	// 	menu_actions::modeSelected();
  //       menu_service::enterTestScreen();
  //     }
  //     break;
    
  //   case TEST_MODE_TURN_ON_GRID_CMD:
  //     printf("MODE TEST GRID\n");
  //     if(deviceOpMode.isInTestStand()) {
  //       service::fakeTemperature = TEST_MODE_GRID_ON_TEMPERATURE;
  //       service::newValueAvailable = true;
  //       menu_service::needUpdate = true;
  //     }
  //     break;
    
  //   case TEST_MODE_TURN_ON_SUN_CMD:
  //     printf("MODE TEST SUN\n");
  //     if(deviceOpMode.isInTestStand()) {
  //       service::fakeTemperature = TEST_MODE_SUN_ON_TEMPERATURE;
  //       service::newValueAvailable = true;
  //       menu_service::needUpdate = true;
  //     }
  //     break;
    
  //   case TEST_MODE_TURN_ON_IDLE_CMD:
  //     printf("MODE TEST IDLE\n");
  //     if(deviceOpMode.isInTestStand()) {  
  //       service::fakeTemperature = TEST_MODE_IDLE_ON_TEMPERATURE;
  //       service::newValueAvailable = true;
  //       menu_service::needUpdate = true;
  //     }
  //     break;
    
  //   default:
  //     printf("MODE bad command\n");
  //     break;
  // }
}

void Rx_interrupt() {
//   while(pc.readable()) {
//     __disable_irq();
//     char rcv = pc.getc();
// //    printf("%c\r\n", rcv);
//     __enable_irq();
//     parseCommand(rcv);
//   }
}

namespace service {
//   float fakeTemperature = TEST_MODE_IDLE_ON_TEMPERATURE;
//   bool newValueAvailable = false;
  
//   void setup() {
//     static const uint32_t pc_baud = SERVICE_COMMS_BAUD_RATE;
    
//     pc.baud(pc_baud);
//     pc.attach(&Rx_interrupt);
//     printf("SERVICE START\n");
// //    printf("[COMMS PC]\n");
// //    printf("[ok] baud %d\n", pc_baud);
//   }
  
//   float getFakeTemperature() {
//     return fakeTemperature;
//   }
  
//   bool isNewValueAvailable() {
}

static uint8_t fakeTemperature = TEST_MODE_IDLE_ON_TEMPERATURE;
static uint8_t newValueAvailable = 0;

void service_setup(void) {
  static const uint32_t pc_baud = SERVICE_COMMS_BAUD_RATE;
}

uint8_t service_getFakeTemperature(void) {
  return fakeTemperature;
}

uint8_t service_newValAvail(void) {
  if (newValueAvailable) {
    newValueAvailable = 0;
    return 1;
  }
  return 0;
}
