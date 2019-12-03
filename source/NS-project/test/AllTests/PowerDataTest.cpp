#include "CppUTest/TestHarness.h"
#include "data.h"

extern "C" {
	/*
	 * Add your c-only include files here
	 */
    #include "consts.h"
}

TEST_GROUP(CleanExit) {
    void setup() {
        PowerData_init();
    }

    void teardown() {
        // PowerData_init();
    }
};

TEST(CleanExit, GET_STATUS_GENERATOR) {
    CHECK_EQUAL(110, NECTAR_POWER_BOARD_VERSION);
    // uint8_t status = (uint8_t)GENERATOR_STATUS;
    // PowerData_write(STATUS, &status);
    // CHECK_TEXT(GET_STATUS(GENERATOR_STATUS), "GENERATOR_STATUS SET");
}
