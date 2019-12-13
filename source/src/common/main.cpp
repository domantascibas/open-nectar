#include "consts.h"
#include "device_modes.h"
#include "main_board_comms.h"
#include "pc_service_comms.h"
#include "watchdog_timer.h"
#include "error_controller.h"
#include "version.h"
#include "flash_storage.h"

extern "C" {
    #include "data.h"
}

// TODO: make main more generic, so it can be shared between different targets

int main(void) {
    error_controller_init();
    pc_service_comms_init();
    PowerData_init();
    flash_storage_init();
    // print device version numbers

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
