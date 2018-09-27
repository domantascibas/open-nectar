#include "consts.h"
#include "flash_storage.h"
#include "device_modes.h"
#include "main_board_comms.h"
#include "pc_service_comms.h"
#include "watchdog_timer.h"

// DigitalOut pa0(PA_0);
// DigitalOut pa1(PA_1);
// // DigitalOut pa2(PA_2);
// // DigitalOut pa3(PA_3);
// DigitalOut pa4(PA_4);
// DigitalOut pa5(PA_5);
// DigitalOut pa6(PA_6);
// DigitalOut pa7(PA_7);
// DigitalOut pa8(PA_8);
// DigitalOut pa9(PA_9);
// DigitalOut pa10(PA_10);
// DigitalOut pa11(PA_11);
// DigitalOut pa12(PA_12);
// //DigitalOut pa13(PA_13);
// //DigitalOut pa14(PA_14);
// //DigitalOut pa15(PA_15);

// DigitalOut pb0(PB_0);
// DigitalOut pb1(PB_1);
// DigitalOut pb2(PB_2);
// DigitalOut pb3(PB_3);
// DigitalOut pb4(PB_4);
// DigitalOut pb5(PB_5);
// DigitalOut pb6(PB_6);
// DigitalOut pb7(PB_7);
// DigitalOut pb8(PB_8);
// DigitalOut pb9(PB_9);
// DigitalOut pb10(PB_10);
// DigitalOut pb11(PB_11);
// DigitalOut pb12(PB_12);
// //DigitalOut pb13(PB_13);
// //DigitalOut pb14(PB_14);
// DigitalOut pb15(PB_15);

// DigitalOut pc0(PC_0);
// DigitalOut pc1(PC_1);
// DigitalOut pc2(PC_2);
// DigitalOut pc3(PC_3);
// DigitalOut pc4(PC_4);
// DigitalOut pc5(PC_5);
// DigitalOut pc6(PC_6);
// //DigitalOut pc7(PC_7);
// DigitalOut pc8(PC_8);
// //DigitalOut pc9(PC_9);
// //DigitalOut pc10(PC_10);
// //DigitalOut pc11(PC_11);
// //DigitalOut pc12(PC_12);
// DigitalOut pc13(PC_13);
// DigitalOut pc14(PC_14);
// DigitalOut pc15(PC_15);

// //DigitalOut pd2(PD_2);

// DigitalOut pf0(PF_0);
// DigitalOut pf1(PF_1);
// DigitalOut pf11(PF_11);

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
