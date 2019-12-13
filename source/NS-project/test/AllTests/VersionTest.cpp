#include "CppUTest/TestHarness.h"
#include "version.h"

TEST_GROUP(VersionTests) {
};

TEST(VersionTests, NSPowerString) {
    STRCMP_EQUAL("NS-Power", Dev_Model);
}
