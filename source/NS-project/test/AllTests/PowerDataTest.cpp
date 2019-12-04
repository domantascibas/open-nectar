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

TEST(PowerDataTests, GET_STATUS) {
    statusDataType_t i;
    uint8_t s, r;
    printf("\n\r");
    for (i = (statusDataType_t)0; i < NUMEL_STATUS;) {
        s = (uint8_t)(1 << i);
        PowerData_write(STATUS, &s);
        PowerData_read(STATUS, &r);
        printf("i: %d , s: %d , r: %d , GET_STATUS(): %d\n\r", i, s, r, GET_STATUS(i));

        CHECK(GET_STATUS(i));
        i = (statusDataType_t)((int)i + 1);
    }
}

TEST(PowerDataTests, SET_CLEAR_STATUS) {
    statusDataType_t i;
    uint8_t r;
    printf("\n\r");
    for (i = (statusDataType_t)0; i < NUMEL_STATUS;) {
        SET_STATUS(i);
        PowerData_read(STATUS, &r);
        CHECK(GET_STATUS(i));
        printf("i: %d , r: %d , GET_STATUS(): %d\n\r", i, r, GET_STATUS(i));

        CLEAR_STATUS(i);
        CHECK_FALSE(GET_STATUS(i));

        i = (statusDataType_t)((int)i + 1);
    }
    PowerData_read(STATUS, &r);
    CHECK_EQUAL(0, r);
}
