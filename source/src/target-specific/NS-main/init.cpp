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

#define BTN_LIGHT_ON 1
#define BTN_LIGHT_OFF 0

DigitalOut lights(MENU_BUTTON_BACKLIGHT_PIN, BTN_LIGHT_OFF);

bool inErrorScreen = false;
Timer comms_timeout;
Thread temperatureThread;

static void temperatureUpdate() {
    while (1) {
        temperatureController_update();
        wait(5);
    }
}

void print_device_info(void) {
    printf("[INFO] product:     %s\r\n", PRODUCT_NAME);
    printf("[INFO] type:        %s\r\n", Dev_Model);
    printf("[INFO] version:     %s\r\n", version);
    printf("[INFO] old ver:     %d\r\n", NECTAR_MAIN_BOARD_VERSION);
    printf("[INFO] hw sign:     %s\r\n", HW_sign);
}

void hw_init(void) {
    error_init();
    // watchdog_timer_init();
    storage_init();
    // temperature_controller_init();
    temperatureController_init();
    // menu_service::setup();
    // menu_service::updateScreen();
}

#define BLINKING_RATE 1

void run(void) {
    wait(5);
    temperatureThread.start(temperatureUpdate);

    while (true) {
        lights = !lights;
        printf("[BTN] lights: %s\r\n", lights ? "ON" : "OFF");
        wait(BLINKING_RATE);
    }
}

// int main() {

//     hw_init();

//     // comms_init
//     esp::setup();
//     power_board::setup();

// }

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

//     deviceOpMode_endLoading();

// temperatureThread.start(temperatureUpdate);
// screenThread.start(screenUpdate);

// Thread::wait(osWaitForever);

// while (1) {
//     watchdog_timer_update();
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

//     __WFI();
// }
// }
