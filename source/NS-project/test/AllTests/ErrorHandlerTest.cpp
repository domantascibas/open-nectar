#include "CppUTest/TestHarness.h"
#include <stdio.h>
#include "error_handler.h"

TEST_GROUP(ErrorHandler) {
    void setup() {
        printf("\r\n***** NS-ERROR TEST *****\r\n");
        uint16_t err = (1U << NS_FLASH_ACCESS_MAIN) | (1U << NS_FLASH_ACCESS_POWER) | (1U << NS_ADC_CURRENT) | (1U << NS_ADC_VOLTAGE) | (1U << NS_CALIBRATION) | (1U << NS_MAX_TEMPERATURE) | (1U << NS_MIN_TEMPERATURE);
        error_init(err);
    }

    void teardown() {
    }
};

TEST(ErrorHandler, Init) {
    error_print();
}

TEST(ErrorHandler, Set) {
    error_set(NS_DEVICE_OVERHEAT);
    error_print();
}

TEST(ErrorHandler, Clear) {
    error_clear(NS_MIN_TEMPERATURE);
    error_clear(NS_MAX_TEMPERATURE);
    error_print();
}
