#include "consts.h"
#include "watchdog_timer.h"

void watchdog_timer_init(void) {
  IWDG->KR = 0xCCCC;
  IWDG->KR = 0x5555;
  IWDG->PR = IWDG_PR_PR_0;
  IWDG->RLR = 0xFFFF;
  while (IWDG->SR) {
  /* add time out here for a robust application */
  }
  IWDG->KR = 0xAAAA;
}

void watchdog_timer_update(void) {
  // #kickTheDog
  IWDG->KR = 0xAAAA;
}