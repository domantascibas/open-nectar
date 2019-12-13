#include "CppUTest/TestHarness.h"

extern "C" {
    #include "consts.h"
    #include "data.h"
}

TEST_GROUP(ConstsTests) {
};

TEST(ConstsTests, IsPowerVersion_110) {
    CHECK_EQUAL(110, NECTAR_POWER_BOARD_VERSION);
}

TEST(ConstsTests, PrintDataInfo) {
    // check min max values

    // M_VOLTAGE,
    // M_CURRENT,
    // M_POWER,
    // R_VOLTAGE,
    // R_CURRENT,
    // GRID_METER,
    // SUN_METER,
    // TIMESTAMP,
    // STATUS,
    // TEMPERATURE,
    uint16_t v, i;
    uint32_t p, r_v, r_i;
    v = 36575;
    i = 1202;
    p = (uint32_t)(v * i);

    r_v = 1603029;      // > 0.001
    r_i = 2230640514;   // < 2.2

    PowerData_write(M_VOLTAGE, &v);
    PowerData_write(M_CURRENT, &i);
    PowerData_write(M_POWER, &p);
    PowerData_write(R_VOLTAGE, &r_v);
    PowerData_write(R_CURRENT, &r_i);

    PowerData_info();
}
