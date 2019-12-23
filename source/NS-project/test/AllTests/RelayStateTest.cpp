#include "CppUTest/TestHarness.h"
#include <stdint.h>
#include <stdio.h>

extern "C" {
#include "RelayState.h"
}

TEST_GROUP (RelayStateTests) {
    void setup() {
        RelayState_init();
    }

    void teardown() {
    }
};

TEST (RelayStateTests, CheckInit) {
    CHECK_EQUAL(OFF, RelayState_get(SUN_RELAY));
}

// check sun relay turns on
// check grid relay turns on
// check that both relays aren't turned on
// check do not allow relay switch if still switching
