#include "mbed.h"
#include "storage.h"
#include "device_modes.h"
#include "main_board.h"
#include "data.h"
#include "service.h"
#include "ErrorHandler.h"

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

void initIndependentWatchdog() {
  /* (1) Activate IWDG (not needed if done in option bytes) */
  /* (2) Enable write access to IWDG registers */
  /* (3) Set prescaler by 8 */
  /* (4) Set reload value to have a rollover each 100ms */
  /* (5) Check if flags are reset */
  /* (6) Refresh counter */
  IWDG->KR = 0xCCCC; /* (1) */
  IWDG->KR = 0x5555; /* (2) */
  IWDG->PR = IWDG_PR_PR_0; /* (3) */
  IWDG->RLR = 0xFFFF; /* (4) */
  while (IWDG->SR) /* (5) */
  {
  /* add time out here for a robust application */
  }
  IWDG->KR = 0xAAAA; /* (6) */
}

void kickTheDog() {
  IWDG->KR = 0xAAAA;
//  printf("kick the dog\r\n");
}

int main() {
  service::setup();
	Storage::init();
  // initInternalTempSensor();
  
  printf("Starting device modes\r\n");
  device_modes::setup();
  printf("Energy Meters: %.4f, %.4f\r\n", data.sun_energy_meter_kwh, data.grid_energy_meter_kwh);
  main_board::setup();
  printf("SETUP DONE\r\n");
  
  initIndependentWatchdog();
  
  while(1) {
    kickTheDog();
    if(!nectarError.has_error(CALIBRATION_ERROR)) device_modes::loop();
    device_modes::calibrate_device();
    __WFI();
  }
}
