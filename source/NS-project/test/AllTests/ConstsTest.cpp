#include "CppUTest/TestHarness.h"

extern "C" {
    #include "consts.h"
}

TEST_GROUP(ConstsTests) {
};

TEST(ConstsTests, IsPowerVersion_110) {
    CHECK_EQUAL(110, NECTAR_POWER_BOARD_VERSION);
}
