#include "CppUTest/TestHarness.h"
#include <stdio.h>

extern "C" {
    #include "data.h"
}

TEST_GROUP(PowerDataTests) {
    void setup() {
        PowerData_init();
    }

    void teardown() {
    }
};

TEST(PowerDataTests, GET_STATUS) {
    statusDataType_t i;
    uint8_t s, r;
    printf("\n\r");
    for (i = (statusDataType_t)0; i < STATUS_TYPE_COUNT;) {
        s = (uint8_t)(1 << i);
        PowerData_write(STATUS, &s);
        PowerData_read(STATUS, &r);
        printf("i: %d , s: %d , r: %d , GET_STATUS(): %d\n\r", i, s, r, GET_STATUS(i));

        CHECK(GET_STATUS(i));
        i = (statusDataType_t)((int)i + 1);
    }
}

TEST(PowerDataTests, SET_CLEAR_STATUS) {
    uint8_t i = 2;
    uint8_t r = 1 << 2;
    printf("\n\r");
    // SET_STATUS(i);
    PowerData_write(STATUS, &r);
    CHECK(GET_STATUS(i));
    printf("i: %d , r: %d , GET_STATUS(): %d\n\r", i, r, GET_STATUS(i));
    CLEAR_STATUS(i);
    CHECK_FALSE(GET_STATUS(i));
    PowerData_read(STATUS, &r);
    printf("i: %d , r: %d , GET_STATUS(): %d\n\r", i, r, GET_STATUS(i));
    CHECK_EQUAL(0, r);
}

// TEST(PowerDataTests, SET_CLEAR_STATUS) {
//     statusDataType_t i;
//     uint8_t r;
//     printf("\n\r");
//     for (i = (statusDataType_t)0; i < NUMEL_STATUS;) {
//         SET_STATUS(i);
//         PowerData_read(STATUS, &r);
//         CHECK(GET_STATUS(i));
//         printf("i: %d , r: %d , GET_STATUS(): %d\n\r", i, r, GET_STATUS(i));

//         CLEAR_STATUS(i);
//         CHECK_FALSE(GET_STATUS(i));

//         i = (statusDataType_t)((int)i + 1);
//     }
//     PowerData_read(STATUS, &r);
//     CHECK_EQUAL(0, r);
// }

// TEST(PowerDataTests, StatusPointer) {
//     statusDataType_t i;
//     uint8_t r;
//     printf("\n\r");
//     for (i = (statusDataType_t)0; i < NUMEL_STATUS;) {
//         r = (uint8_t)(1 << i);
//         SET_STATUS(i);
//         CHECK_EQUAL(r, *PowerData_getStatusPtr());
//         printf("i: %d , r: %d , PwrPtr(): %d\n\r", i, r, *PowerData_getStatusPtr());
//         CLEAR_STATUS(i);
//         i = (statusDataType_t)((int)i + 1);
//     }
// }

TEST(PowerDataTests, WriteReadFloat_momentData) {
    float v[] = {0.01f, 0.39f, 10.01f, 3.25f, 3.67f, 384.65f, 500.68f, 100.10f};
    float i[] = {0.23f, 1.25f, 5.85f, 12.1f};
    float f, g;
    uint8_t k, l;
    uint16_t c, m, r, s;
    uint32_t n, t;

    for (l = 0; l < (sizeof(i) / sizeof(float)); l++) {
        for (k = 0; k < (sizeof(v) / sizeof(float)); k++) {
            f = v[k];
            g = i[l];
            c = (uint16_t)(f * 100);
            PowerData_write(M_VOLTAGE, &c);
            m = (uint16_t)(g * 100);
            PowerData_write(M_CURRENT, &m);
            n = (uint32_t)c * (uint32_t)m;
            PowerData_write(M_POWER, &n);

            PowerData_read(M_VOLTAGE, &r);
            PowerData_read(M_CURRENT, &s);
            PowerData_read(M_POWER, &t);
            
            CHECK_EQUAL(f, (float)r / 100);
            CHECK_EQUAL(g, (float)s / 100);
            // CHECK_EQUAL((f * g * 10000), t);
            // PowerData_info();
            // printf("f: %f , g: %f , f * g: %f , t: %f\n\r", f, g, f*g, (float)t / 10000);
        }
    }
}
