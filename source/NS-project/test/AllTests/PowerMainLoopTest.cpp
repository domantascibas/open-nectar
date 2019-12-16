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

void gotoPwmIdle(void) {
    set_variables(101.0f, 0.1f, ON);
    MainController_run();
}

void gotoMpptRun(void) {
    set_variables(101.0f, 0.1f, ON);
    MainController_run();
    set_variables(50.0f, 0.3f, ON);
    MainController_run();
}

uint8_t checkStatus(uint8_t gen, uint8_t pwm, uint8_t mppt) {
    CHECK_EQUAL(gen, GET_STATUS(GENERATOR_STATUS));
    CHECK_EQUAL(pwm, GET_STATUS(PWM_STATUS));
    CHECK_EQUAL(mppt, GET_STATUS(MPPT_STATUS));
    return 1;
}

TEST_GROUP(PowerMainLoopIdleTests){
    void setup() {
        PowerData_init();
        MainController_init();
    }

    void teardown() {
    }
};

TEST_GROUP(PowerMainPwmIdleLoopTests){
    void setup() {
        PowerData_init();
        MainController_init();
        gotoPwmIdle();
    }

    void teardown() {
    }
};

TEST_GROUP(PowerMainMpptRunLoopTests){
    void setup() {
        PowerData_init();
        MainController_init();
        gotoMpptRun();
    }

    void teardown() {
    }
};


/* IDLE STATE */
// return to IDLE
TEST(PowerMainLoopIdleTests, PowerIdleState_FAIL_1) {
    set_variables(9.0f, 0.1f, OFF);
    CHECK_EQUAL(IDLE_STATE, MainController_run());
    checkStatus(0, 0, 0);
}

// return to IDLE
TEST(PowerMainLoopIdleTests, PowerIdleState_FAIL_2) {
    set_variables(101.0f, 0.1f, OFF);
    CHECK_EQUAL(IDLE_STATE, MainController_run());
    checkStatus(0, 0, 0);
}

// return to IDLE
TEST(PowerMainLoopIdleTests, PowerIdleState_FAIL_3) {
    set_variables(9.0f, 0.1f, ON);
    CHECK_EQUAL(IDLE_STATE, MainController_run());
    checkStatus(0, 0, 0);
}

// go to PWM idle
TEST(PowerMainLoopIdleTests, PowerIdleState_PASS) {
    set_variables(101.0f, 0.1f, ON);
    CHECK_EQUAL(PWM_IDLE_STATE, MainController_run());
    checkStatus(1, 0, 0);
}


/* PWM IDLE STATE */
// return to IDLE
TEST(PowerMainPwmIdleLoopTests, PowerPwmIdleState_FAIL_1_returnToIdle) {
    set_variables(50.0f, 0.1f, OFF);
    CHECK_EQUAL(RETURN_TO_IDLE, MainController_run());
    CHECK_EQUAL(IDLE_STATE, MainController_run());
    checkStatus(0, 0, 0);
}

// return to IDLE
TEST(PowerMainPwmIdleLoopTests, PowerPwmIdleState_FAIL_2_returnToIdle) {
    set_variables(9.0f, 0.1f, OFF);
    CHECK_EQUAL(RETURN_TO_IDLE, MainController_run());
    CHECK_EQUAL(IDLE_STATE, MainController_run());
    checkStatus(0, 0, 0);
}

// stay at PWM IDLE
TEST(PowerMainPwmIdleLoopTests, PowerPwmIdleState_STAY) {
    set_variables(50.0f, 0.1f, ON);
    CHECK_EQUAL(PWM_IDLE_STATE, MainController_run());
    checkStatus(1, 0, 0);
}

// return to IDLE
TEST(PowerMainPwmIdleLoopTests, PowerPwmIdleState_FAIL_3_returnToIdle) {
    set_variables(50.0f, 0.3f, OFF);
    CHECK_EQUAL(RETURN_TO_IDLE, MainController_run());
    CHECK_EQUAL(IDLE_STATE, MainController_run());
    checkStatus(0, 0, 0);
}

// return to IDLE
TEST(PowerMainPwmIdleLoopTests, PowerPwmIdleState_FAIL_4_returnToIdle) {
    set_variables(9.0f, 0.3f, OFF);
    CHECK_EQUAL(RETURN_TO_IDLE, MainController_run());
    CHECK_EQUAL(IDLE_STATE, MainController_run());
    checkStatus(0, 0, 0);
}

// go to MPPT start
TEST(PowerMainPwmIdleLoopTests, PowerPwmIdleState_PASS) {
    set_variables(50.0f, 0.3f, ON);
    CHECK_EQUAL(MPPT_RUN_STATE, MainController_run());
    checkStatus(1, 1, 1);
}


/* MPPT RUN STATE */
// return to IDLE
TEST(PowerMainMpptRunLoopTests, PowerMpptRunState_FAIL_1_returnToIdle) {
    set_variables(50.0f, 0.1f, OFF);
    CHECK_EQUAL(RETURN_TO_IDLE, MainController_run());
    CHECK_EQUAL(IDLE_STATE, MainController_run());
    checkStatus(0, 0, 0);
}

// return to IDLE
TEST(PowerMainMpptRunLoopTests, PowerMpptRunState_FAIL_2_returnToIdle) {
    set_variables(9.0f, 0.1f, OFF);
    CHECK_EQUAL(RETURN_TO_IDLE, MainController_run());
    CHECK_EQUAL(IDLE_STATE, MainController_run());
    checkStatus(0, 0, 0);
}

// stay at MPPT RUN
TEST(PowerMainMpptRunLoopTests, PowerMpptRunState_STAY) {
    set_variables(50.0f, 0.1f, ON);
    CHECK_EQUAL(MPPT_RUN_STATE, MainController_run());
    checkStatus(1, 1, 1);
}

// return to IDLE
TEST(PowerMainMpptRunLoopTests, PowerMpptRunState_FAIL_3_returnToIdle) {
    set_variables(50.0f, 0.3f, OFF);
    CHECK_EQUAL(RETURN_TO_IDLE, MainController_run());
    CHECK_EQUAL(IDLE_STATE, MainController_run());
    checkStatus(0, 0, 0);
}

// return to IDLE
TEST(PowerMainMpptRunLoopTests, PowerMpptRunState_FAIL_4_returnToIdle) {
    set_variables(9.0f, 0.3f, OFF);
    CHECK_EQUAL(RETURN_TO_IDLE, MainController_run());
    CHECK_EQUAL(IDLE_STATE, MainController_run());
    checkStatus(0, 0, 0);
}

// stay at MPPT RUN
TEST(PowerMainMpptRunLoopTests, PowerMpptRunState_STAY_2) {
    set_variables(50.0f, 0.3f, ON);
    CHECK_EQUAL(MPPT_RUN_STATE, MainController_run());
    checkStatus(1, 1, 1);
}
