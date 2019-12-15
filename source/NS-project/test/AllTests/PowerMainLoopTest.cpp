#include "CppUTest/TestHarness.h"
#include <stdint.h>

extern "C" {
#include "data.h"
#include "main-controller.h"
}

#define OFF 0
#define ON 1

void set_variables(float voltage, float current, uint8_t sun_status) {
    uint16_t v = (uint16_t)(voltage * 100);
    uint16_t i = (uint16_t)(current * 100);

    PowerData_write(M_VOLTAGE, &v);
    PowerData_write(M_CURRENT, &i);
    SunStatus_set(sun_status);
}

void generator_on(void) {
    SET_STATUS(GENERATOR_STATUS);
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
    // call mainController_loop periodically, change input variables
    // set voltage = 10V * 100
    // set sun status OFF
    // I = 0.1 * 100
    // 
    set_variables(10.0f, 0.1f, OFF);

}
