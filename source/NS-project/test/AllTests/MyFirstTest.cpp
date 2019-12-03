#include "CppUTest/TestHarness.h"

extern "C" {
	/*
	 * Add your c-only include files here
	 */
}

TEST_GROUP(MyTest) {
    void setup() {
    }

    void teardown() {
    }
};

TEST(MyTest, test1) {
//    FAIL("Your test is running! Now delete this line and watch your test pass.");
    /*
     * Instantiate your class, or call the function, you want to test.
     * Then delete this comment
     */
}

TEST(MyTest, test2) {
    // STRCMP_EQUAL("hello", "world"); // fails
    STRCMP_EQUAL("hello", "hello");
}
