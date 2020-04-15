#include "consts.h"
#include "ServiceComms.h"
#include "DeviceMode.h"
// #include "menu_service.h"
#include "pins.h"

RawSerial pc(SERVICE_COMMS_TX_PIN, SERVICE_COMMS_RX_PIN);

static uint8_t fakeTemperature = TEST_MODE_IDLE_ON_TEMPERATURE;
static uint8_t newValueAvailable = 0;

void parseCommand(uint8_t command) {
    switch (command) {
        case TEST_MODE_ENTER_TEST_MODE_CMD:
            printf("MODE TEST\n");
            if (!deviceOpMode_isInTestStand()) {
                deviceOpMode_setInTestStand();
                fakeTemperature = TEST_MODE_IDLE_ON_TEMPERATURE;
                newValueAvailable = 1;
                // menu_actions::modeSelected();
                // menu_service::enterTestScreen();
            }
            break;

        case TEST_MODE_TURN_ON_GRID_CMD:
            printf("MODE TEST GRID\n");
            if (deviceOpMode_isInTestStand()) {
                fakeTemperature = TEST_MODE_GRID_ON_TEMPERATURE;
                newValueAvailable = 1;
                // menu_service::needUpdate = true;
            }
            break;

        case TEST_MODE_TURN_ON_SUN_CMD:
            printf("MODE TEST SUN\n");
            if (deviceOpMode_isInTestStand()) {
                fakeTemperature = TEST_MODE_SUN_ON_TEMPERATURE;
                newValueAvailable = 1;
                // menu_service::needUpdate = true;
            }
            break;

        case TEST_MODE_TURN_ON_IDLE_CMD:
            printf("MODE TEST IDLE\n");
            if (deviceOpMode_isInTestStand()) {
                fakeTemperature = TEST_MODE_IDLE_ON_TEMPERATURE;
                newValueAvailable = 1;
                // menu_service::needUpdate = true;
            }
            break;

        default:
            printf("MODE bad command\n");
            break;
    }
}

void Rx_interrupt() {
    while (pc.readable()) {
        __disable_irq();
        char rcv = pc.getc();
//    printf("%c\r\n", rcv);
        __enable_irq();
        parseCommand(rcv);
    }
}

void service_setup(void) {
    pc.baud(SERVICE_COMMS_BAUD_RATE);
    printf("PC serial @%d", SERVICE_COMMS_BAUD_RATE);
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
