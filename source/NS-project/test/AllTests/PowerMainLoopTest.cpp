#include "CppUTest/TestHarness.h"
#include <stdint.h>
#include <stdio.h>

extern "C" {
#include "data.h"
#include "main-controller.h"
}

#define OFF 0
#define ON 1

void set_variables(float voltage, float current, uint8_t sun_status) {
    uint16_t v = VI_CONVERT(voltage);
    uint16_t i = VI_CONVERT(current);

    PowerData_write(M_VOLTAGE, &v);
    PowerData_write(M_CURRENT, &i);
    if (sun_status) {
        SET_STATUS(SUN_STATUS);
    } else {
        CLEAR_STATUS(SUN_STATUS);
    }
    // printf("\n\r[TEST] voltage %.2f, current %.2f\n\r", DIV_VI_CONVERT(v), DIV_VI_CONVERT(i));
}

TEST_GROUP(PowerMainLoopTests){
    void setup() {
        PowerData_init();
        MainController_init();
    }

    void teardown() {
    }
};

TEST(PowerMainLoopTests, PowerIdleState) {
    set_variables(10.0f, 0.1f, OFF);
    CHECK_EQUAL(IDLE_STATE, MainController_run());
    CHECK_FALSE(GET_STATUS(GENERATOR_STATUS));
    CHECK_FALSE(GET_STATUS(PWM_STATUS));
    CHECK_FALSE(GET_STATUS(MPPT_STATUS));

    set_variables(101.0f, 0.1f, OFF);
    CHECK_EQUAL(IDLE_STATE, MainController_run());
    CHECK_FALSE(GET_STATUS(GENERATOR_STATUS));
    CHECK_FALSE(GET_STATUS(PWM_STATUS));
    CHECK_FALSE(GET_STATUS(MPPT_STATUS));
    
    set_variables(10.0f, 0.1f, ON);
    CHECK_EQUAL(IDLE_STATE, MainController_run());
    CHECK_FALSE(GET_STATUS(GENERATOR_STATUS));
    CHECK_FALSE(GET_STATUS(PWM_STATUS));
    CHECK_FALSE(GET_STATUS(MPPT_STATUS));
    
    set_variables(101.0f, 0.1f, ON);
    CHECK_EQUAL(PWM_ON_STATE, MainController_run());
    CHECK(GET_STATUS(GENERATOR_STATUS));
    CHECK(GET_STATUS(PWM_STATUS));
    CHECK_FALSE(GET_STATUS(MPPT_STATUS));

    // PWM ON state
    set_variables(101.0f, 0.1f, OFF);
    CHECK_EQUAL(PWM_ON_STATE, MainController_run());
    CHECK(GET_STATUS(GENERATOR_STATUS));
    CHECK(GET_STATUS(PWM_STATUS));
    CHECK_FALSE(GET_STATUS(MPPT_STATUS));

    set_variables(9.0f, 0.1f, OFF);
    CHECK_EQUAL(PWM_ON_STATE, MainController_run());
    CHECK(GET_STATUS(GENERATOR_STATUS));
    CHECK(GET_STATUS(PWM_STATUS));
    CHECK_FALSE(GET_STATUS(MPPT_STATUS));
    
    set_variables(101.0f, 0.1f, ON);
    CHECK_EQUAL(PWM_ON_STATE, MainController_run());
    CHECK(GET_STATUS(GENERATOR_STATUS));
    CHECK(GET_STATUS(PWM_STATUS));
    CHECK_FALSE(GET_STATUS(MPPT_STATUS));
    
    set_variables(101.0f, 0.3f, OFF);
    CHECK_EQUAL(PWM_ON_STATE, MainController_run());
    CHECK(GET_STATUS(GENERATOR_STATUS));
    CHECK(GET_STATUS(PWM_STATUS));
    CHECK_FALSE(GET_STATUS(MPPT_STATUS));

    set_variables(9.0f, 0.3f, OFF);
    CHECK_EQUAL(PWM_ON_STATE, MainController_run());
    CHECK(GET_STATUS(GENERATOR_STATUS));
    CHECK(GET_STATUS(PWM_STATUS));
    CHECK_FALSE(GET_STATUS(MPPT_STATUS));

    set_variables(101.0f, 0.3f, ON);
    CHECK_EQUAL(MPPT_RUN_STATE , MainController_run());
    CHECK(GET_STATUS(GENERATOR_STATUS));
    CHECK(GET_STATUS(PWM_STATUS));
    CHECK(GET_STATUS(MPPT_STATUS));
}
