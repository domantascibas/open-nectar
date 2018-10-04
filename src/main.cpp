#include "consts.h"
#include "flash_storage.h"
#include "device_modes.h"
#include "main_board_comms.h"
#include "pc_service_comms.h"
#include "watchdog_timer.h"

int main(void) {
  pc_service_comms_init();
	flash_storage_init();

  printf("Starting device modes\r\n");
  device_modes_init();
  main_board_comms_init();
  printf("SETUP DONE\r\n");
  
  watchdog_timer_init();
  
  while(1) {
    watchdog_timer_update();
    device_modes_loop();
    __WFI();
  }
}
