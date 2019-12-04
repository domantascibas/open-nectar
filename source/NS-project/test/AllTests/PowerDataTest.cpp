#include "CppUTest/TestHarness.h"

#include "data.h"

// extern "C" {
// }

TEST_GROUP(PowerDataTests) {
    void setup() {
        PowerData_init();
    }

    void teardown() {
        PowerData_init();
    }
};

TEST(PowerDataTests, GET_STATUS_GENERATOR) {
    uint8_t status = (uint8_t)GENERATOR_STATUS;
    PowerData_write(STATUS, &status);
    CHECK_TEXT(GET_STATUS(GENERATOR_STATUS), "GENERATOR_STATUS SET");
}
