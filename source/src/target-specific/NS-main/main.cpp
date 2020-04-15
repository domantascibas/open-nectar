#include "consts.h"
#include "version.h"
#include "ServiceComms.h"
#include "EspComms.h"
#include "PowerBoardComms.h"
#include "device_modes.h"
#include "menu_service.h"
#include "TemperatureController.h"
#include "DataService.h"
#include "Storage.h"
#include "CommsController.h"
#include "watchdog_timer.h"
#include "DeviceMode.h"

// extern "C" {
#include "error_handler.h"
// }

bool inErrorScreen = false;
Timer comms_timeout;

static void print_device_info(void) {
    printf("[INFO] product:     %s\r\n", PRODUCT_NAME);
    printf("[INFO] type:        %s\r\n", Dev_Model);
    printf("[INFO] version:     %s\r\n", version);
    printf("[INFO] old version: %d\r\n", NECTAR_MAIN_BOARD_VERSION);
    printf("[INFO] hw sign:     %s\r\n", HW_sign);
    printf("[INFO] pc baud:     %d\r\n", PC_BAUD);
}

static void hw_init(void) {
    storage_init();
}

int main() {
    printf("\r\n");
    printf("\r\n***** START *****\r\n");
    print_device_info();
    error_init();

    hw_init();


//     float current_s;
//     static bool isFirst = true;
//     deviceOpMode_init();

//     menu_service::setup();
//     menu_service::updateScreen();
//     wait(1.0);
//     temperatureController_init();
//     wait(0.5);
//     temperatureController_update();
//     wait(0.5);

// menu_service::updateScreen();
//     wait(0.5);
//     esp::setup();
//     wait(0.5);
//     power_board::setup();

//     comms_timeout.start();
//     current_s = comms_timeout.read();
//     while ((!power_board::receivedFirstMessage() || (powerData.power_version == 0.00)) && (current_s < POWER_INIT_TIMEOUT)) {
//         __WFI();
//         current_s = comms_timeout.read();
//     }
//     comms_timeout.stop();
//     comms_timeout.reset();
// menu_service::updateScreen();
//     wait(1.5);

// menu_service::updateScreen();
//     comms_timeout.start();
//     current_s = comms_timeout.read();
//     while ((!esp::receivedFirstMessage() || (espData.esp_version == 0.00)) && (current_s < ESP_INIT_TIMEOUT)) {
//         __WFI();
//         current_s = comms_timeout.read();
//     }
//     comms_timeout.stop();
//     comms_timeout.reset();
// menu_service::updateScreen();
//     wait(1.5);

// menu_service::updateScreen();
//     device_modes_setup();
//     wait(2);

//     watchdog_timer_init();
//     deviceOpMode_endLoading();

//     while (1) {
//         watchdog_timer_update();

//         if (isFirst) {
//             isFirst = false;
//             if (storage_isConfigured()) {
//                 printf("CONFIG loading data from storage\r\n");
//                 storage_loadConfigData();
//                 if (storage_isEspConfigured()) {
//                     espData.is_configured = true;
//                     deviceOpMode_setConfigured();
//                 }
//                 deviceOpMode_endOnboarding();
//             }
//     menu_service::needUpdate = true;
//         }

//   if(menu_service::needUpdate) {
//     menu_service::updateScreen();
//   }

//         switch (deviceOpMode_getCurrentMode()) {
//             default:
//             case NOT_CONFIGURED:        // ESP NOT CONFIGURED
//                 if (espData.is_configured) {
//                     printf("CONFIG SAVE\n");
//                     deviceOpMode_setConfigured();
// // menu_service::needUpdate = true;
// // menu_service::resetScreen = true;
//                     storage_saveEspConfig();
//                 }
//                 break;

//             case CONFIGURED:            // ESP CONFIGURED
//                 if (!espData.is_configured) {
//                     printf("CONFIG RESET\n");
//                 }
//                 break;

//             case WELCOME:
// //nothing to do here
// //wait for user to finish onboarding
//                 if (espData.is_configured || storage_isEspConfigured()) {
//                     deviceOpMode_endOnboarding();
//                     printf("CONFIG HAS_CONFIG\n");
// // menu_service::needUpdate = true;
// // menu_service::resetScreen = true;
//                 }
//                 break;

//             case TEST_MODE:
//                 if (isFirst) {
//                     isFirst = false;
//                     deviceOpMode_setTestMode();
//                 }
//                 break;
//         }

//         temperatureController_update();
//         if (!deviceOpMode_isOnboarding()) {
//             device_modes_loop();
//         }

//         commsController.clearQueue();

//         __WFI();
//     }
}
